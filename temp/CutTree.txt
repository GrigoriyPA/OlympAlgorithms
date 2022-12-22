struct Node {
	Node* l = nullptr, * r = nullptr;
	bool update_flag;
	long long sm, ln;
	long long p0, p1;
	long long update;

	Node(long long x = 0) {
		sm = x;
		ln = 1;
		p0 = 0;
		p1 = 0;
		update = 0;
		update_flag = false;
	}

	void change(long long delt) {
		sm += delt;
	}

	Node unite(Node l, Node r) {
		sm = l.sm + l.p0 * l.ln + l.p1 * l.ln * (l.ln - 1) / 2 +
			 r.sm + r.p0 * r.ln + r.p1 * r.ln * (r.ln - 1) / 2;
		ln = l.ln + r.ln;
		return *this;
	}

	void unite(Node* l, Node* r) {
		sm = 0;
		ln = 0;
		if (l != nullptr) {
			sm += l->sm + l->p0 * l->ln + l->p1 * l->ln * (l->ln - 1) / 2;
			ln += l->ln;
		}
		if (r != nullptr) {
			sm += r->sm + r->p0 * r->ln + r->p1 * r->ln * (r->ln - 1) / 2;
			ln += r->ln;
		}
	}

	void divide_updete() {
		update_flag = false;
		if (ln > 1) {
			if (l == nullptr)
				l = new Node;
			l->update_flag = true;
			l->update = update;
			l->sm = update * l->ln;
			l->p0 = 0;
			l->p1 = 0;

			if (r == nullptr)
				r = new Node;
			r->update_flag = true;
			r->update = update;
			r->sm = update * r->ln;
			r->p0 = 0;
			r->p1 = 0;
		}
	}

	void divide_polynom() {
		if (ln > 1) {
			if (l == nullptr)
				l = new Node;
			l->p0 += p0;
			l->p1 += p1;

			if (r == nullptr)
				r = new Node;
			r->p0 += p0;
			r->p1 += p1;
		}
		p0 = 0;
		p1 = 0;
	}
};

class CutTree {
public:
	CutTree(int size) {
		t.resize(1);
		sz = size;
	}

	CutTree(vector < long long > v) {
		t.resize(1);
		sz = v.size();
		for (int i = 0; i < sz; i++)
			update(i, v[i]);
	}


	long long operator [](int index) {
		Node res = get_result(&t[0], 0, sz - 1, index, index);
		return res.sm + res.p0 * res.ln + res.p1 * res.ln * (res.ln - 1) / 2;
	}

	long long get_element(int start_id, int index) {
		Node res = get_result(&t[start_id], 0, sz - 1, index, index);
		return res.sm + res.p0 * res.ln + res.p1 * res.ln * (res.ln - 1) / 2;
	}

	long long get_sum(int l, int r, int start_id = 0) {
		Node res = get_result(&t[start_id], 0, sz - 1, l, r);
		return res.sm + res.p0 * res.ln + res.p1 * res.ln * (res.ln - 1) / 2;
	}


	void update(int pos, long long val, int start_id = 0) {
		update(&t[start_id], 0, sz - 1, pos, val);
	}

	void change(int pos, long long delt, int start_id = 0) {
		update(&t[start_id], 0, sz - 1, pos, delt, true);
	}

	void update_cut(int l, int r, long long val, int start_id = 0) {
		update_cut(&t[start_id], 0, sz - 1, l, r, val);
	}

	void change_cut(int l, int r, long long p0, long long p1 = 0, int start_id = 0) {
		change_cut(&t[start_id], 0, sz - 1, l, r, p0, p1);
	}

	int persistent_update(int pos, long long val, int start_id = 0) {
		t.push_back(Node());
		persistent_update(&t[start_id], &t.back(), 0, sz - 1, pos, val);
		return t.size() - 1;
	}

private:
	vector < Node > t;
	int sz;

	Node get_result(Node* v, int l, int r, int i, int j, long long p0 = 0, long long p1 = 0) {
		if (v->update_flag)
			v->divide_updete();

		if (l == i && r == j) {
			Node res = *v;
			res.p0 += p0;
			res.p1 += p1;
			return res;
		}

		int md = (l + r) / 2;
		p0 += v->p0;
		p1 += v->p1;
		if (j <= md) {
			if (v->l == nullptr)
				v->l = new Node;
			return get_result(v->l, l, md, i, j, p0, p1);
		}
		else if (md < i) {
			if (v->r == nullptr)
				v->r = new Node;
			return get_result(v->r, md + 1, r, i, j, p0 + p1 * (md + 1 - l), p1);
		}
		else {
			if (v->l == nullptr)
				v->l = new Node;
			if (v->r == nullptr)
				v->r = new Node;
			return Node().unite(get_result(v->l, l, md, i, md, p0, p1), get_result(v->r, md + 1, r, md + 1, j, p0 + p1 * (md + 1 - l), p1));
		}
	}

	void update(Node* v, int l, int r, int pos, long long val, bool change = false) {
		if (v->update_flag) 
			v->divide_updete();
		
		if (l == r) {
			if (change)
				v->change(val);
			else {
				long long p0 = v->p0, p1 = v->p1;
				*v = Node(val);
				v->p0 = p0;
				v->p1 = p1;
			}
			return;
		}

		int md = (l + r) / 2;
		if (pos <= md) {
			if (v->l == nullptr)
				v->l = new Node;
			update(v->l, l, md, pos, val, change);
		}
		else {
			if (v->r == nullptr)
				v->r = new Node;
			update(v->r, md + 1, r, pos, val, change);
		}
		v->unite(v->l, v->r);
	}

	void update_cut(Node* v, int l, int r, int i, int j, long long val) {
		if (v->update_flag)
			v->divide_updete();
		if (v->p0 || v->p1)
			v->divide_polynom();

		if (l == i && r == j) {
			v->update_flag = true;
			v->update = val;
			v->sm = val * v->ln;
			return;
		}

		int md = (l + r) / 2;
		if (j <= md) {
			if (v->l == nullptr)
				v->l = new Node;
			update_cut(v->l, l, md, i, j, val);
		}
		else if (md < i) {
			if (v->r == nullptr)
				v->r = new Node;
			update_cut(v->r, md + 1, r, i, j, val);
		}
		else {
			if (v->l == nullptr)
				v->l = new Node;
			if (v->r == nullptr)
				v->r = new Node;
			update_cut(v->l, l, md, i, md, val);
			update_cut(v->r, md + 1, r, md + 1, j, val);
		}
		v->unite(v->l, v->r);
	}

	void change_cut(Node* v, int l, int r, int i, int j, long long p0, long long p1) {
		if (v->update_flag) 
			v->divide_updete();
		
		if (l == i && r == j) {
			v->p0 += p0;
			v->p1 += p1;
			return;
		}

		int md = (l + r) / 2;
		if (j <= md) {
			if (v->l == nullptr)
				v->l = new Node;
			change_cut(v->l, l, md, i, j, p0, p1);
		}
		else if (md < i) {
			if (v->r == nullptr)
				v->r = new Node;
			change_cut(v->r, md + 1, r, i, j, p0, p1);
		}
		else {
			if (v->l == nullptr)
				v->l = new Node;
			if (v->r == nullptr)
				v->r = new Node;
			change_cut(v->l, l, md, i, md, p0, p1);
			change_cut(v->r, md + 1, r, md + 1, j, p0 + p1 * (md + 1 - i), p1);
		}
		v->unite(v->l, v->r);
	}

	void persistent_update(Node* m, Node* n, int l, int r, int pos, long long val) {
		if (l == r) {
			*n = Node(val);
			return;
		}

		int md = (l + r) / 2;
		if (pos <= md) {
			n->l = new Node;
			n->r = m->r;
			if (m->l == nullptr)
				m->l = new Node;
			persistent_update(m->l, n->l, l, md, pos, val);
		}
		else {
			n->r = new Node;
			n->l = m->l;
			if (m->r == nullptr)
				m->r = new Node;
			persistent_update(m->r, n->r, md + 1, r, pos, val);
		}
		n->unite(n->l, n->r);
	}
};