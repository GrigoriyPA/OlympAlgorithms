#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <math.h>
using namespace std;
 
#define double long long
const double inf = 1000000000000, pi = 3.14159265, eps = 0.0001;
 
 
short sign(double x)
{
	if (x > eps)
	{
		return  1;
	}
	if (x < -eps)
	{
		return -1;
	}
	return 0;
}
 
 
bool eqt(double x, double y)
{
	return x >= y - eps && x <= y + eps;
}
 
 
int gcd(int a, int b)
{
	int tmp;
	while (b > 0)
	{
		tmp = a;
		a = b;
		b = tmp % b;
	}
	return a;
}
 
 
// Класс, отвечающий за работу с векторми (3D)
class Vect
{
public:
	double x, y, z; // Координаты вектора в декартовой системе координат
	int id;
 
	// Конструктор
	Vect(double x, double y, double z = 0)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}
 
	// Конструктор по умолчанию
	Vect()
	{
		x = 0;
		y = 0;
		z = 0;
	}
 
 
	// Перегрузка оператора сравнения для Vect != Vect
	bool operator !=(Vect other)
	{
		return x != other.x || y != other.y || z != other.z;
	}
 
	// Перегрузка оператора сравнения для Vect == Vect
	bool operator ==(Vect other)
	{
		return x == other.x && y == other.y && z == other.z;
	}
 
 
	// Перегрузка оператора сложения для Vect += Vect
	void operator +=(Vect other)
	{
		x += other.x;
		y += other.y;
		z += other.z;
	}
 
	// Перегрузка оператора вычитания для Vect -= Vect
	void operator -=(Vect other)
	{
		x -= other.x;
		y -= other.y;
		z -= other.z;
	}
 
	// Перегрузка оператора умножения для Vect *= double
	void operator *=(double other)
	{
		x *= other;
		y *= other;
		z *= other;
	}
 
	// Перегрузка оператора деления для Vect /= double
	void operator /=(double other)
	{
		if (other != 0)
		{
			x /= other;
			y /= other;
			z /= other;
		}
		else
		{
			x = inf;
			y = inf;
			z = inf;
		}
	}
 
 
	// Перегрузка оператора унарный минус для -Vect
	inline Vect operator -()
	{
		return Vect(-x, -y, -z);
	}
 
 
	// Перегрузка оператора сложения для Vect + Vect
	inline Vect operator +(Vect other)
	{
		return Vect(x + other.x, y + other.y, z + other.z);
	}
 
	// Перегрузка оператора вычитания для Vect - Vect
	inline Vect operator -(Vect other)
	{
		return Vect(x - other.x, y - other.y, z - other.z);
	}
 
	// Перегрузка оператора умножения для Vect * double
	inline Vect operator *(double other)
	{
		return Vect(x * other, y * other, z * other);
	}
 
	// Перегрузка оператора умножения (скалярное произведение) для Vect * Vect
	inline double operator *(Vect other)
	{
		return x * other.x + y * other.y + z * other.z;
	}
 
	// Перегрузка оператора ^ (векторное произведение) для Vect ^ Vect
	inline Vect operator ^(Vect other)
	{
		return Vect(y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x);
	}
 
	// Перегрузка оператора деления для Vect / double
	Vect operator /(double other)
	{
		if (other != 0)
		{
			return Vect(x / other, y / other, z / other);
		}
		return Vect(inf, inf, inf);
	}
 
 
	// Получить длину вектора this
	inline double length()
	{
		return sqrt(*this * *this);
	}
 
	// Нормализировать (привести длину к единице) вектор this
	inline Vect normalization()
	{
		return *this / (*this).length();
	}
 
 
	// Получить косинус угла между векторами this, V
	inline double Сos_angle(Vect V)
	{
		return (*this * V) / ((*this).length() * V.length());
	}
 
	// Получить синус угла между векторами this, V
	inline double Sin_angle(Vect V)
	{
		return ((*this ^ V).length()) / ((*this).length() * V.length());
	}
 
	// Отразить вектор относительно нормали N
	inline Vect Reflect_vect(Vect N)
	{
		return N * (N * *this) * 2 - *this;
	}
 
 
	Vect Refract_vect(Vect N, double n_obj, double n_env)
	{
		double cos_alf = (-*this).Сos_angle(N);
		Vect a = N * (N * *this);
		Vect x = *this - a;
		if (cos_alf >= 0)
		{
			Vect y = x * (n_env / n_obj);
			Vect b = a * (sqrt(*this * *this - y * y) / a.length());
			return b + y;
		}
		Vect y = x * (n_obj / n_env);
		if ((*this).length() >= y.length())
		{
			Vect b = a * (sqrt(*this * *this - y * y) / a.length());
			return b + y;
		}
		return Vect();
	}
 
	// Для каждой компоненты установить максимум между компонентой вектора this и V
	void Set_max(Vect V)
	{
		x = max(x, V.x);
		y = max(y, V.y);
		z = max(z, V.z);
	}
 
	// Для каждой компоненты установить минимум между компонентой вектора this и V
	void Set_min(Vect V)
	{
		x = min(x, V.x);
		y = min(y, V.y);
		z = min(z, V.z);
	}
};
 
 
// Класс, отвечающий за работу с линиями
class Line
{
public:
	Vect N, P1, P2;
	double d;
 
	Line(Vect P1, Vect P2)
	{
		this->P1 = P1;
		this->P2 = P2;
 
		N = Vect((P2 - P1).y, -(P2 - P1).x);
 
		d = -(P1 * N);
	}
 
	Line(double A, double B, double C)
	{
		N = Vect(A, B);
		d = C / N.length();
		N = N.normalization();
 
		if (N.x == 0)
		{
			P1 = Vect(1, -d / N.y);
			P2 = Vect(-1, -d / N.y);
		}
		else
		{
			P1 = Vect((-d - N.y) / N.x, 1);
			P2 = Vect((-d + N.y) / N.x, -1);
		}
	}
 
	Line()
	{
		d = 0;
	}
 
	bool check_aff(Vect P)
	{
		return eqt(N * P + d, 0);
	}
 
	Vect intersect(Line l)
	{
		double a1 = N.x, b1 = N.y, c1 = d;
		double a2 = l.N.x, b2 = l.N.y, c2 = l.d;
 
		if (eqt(a1 * b2 - a2 * b1, 0))
		{
			return Vect(inf, inf);
		}
 
		return -Vect((c1 * b2 - c2 * b1) / (a1 * b2 - a2 * b1), (a1 * c2 - a2 * c1) / (a1 * b2 - a2 * b1));
	}
};
 
 
// Проверка принадлежности отрезку, еси точка лежит на его прямой
bool cut(Vect a, Vect b, Vect P)
{
	return (P - a) * (P - b) < 0 || eqt((P - a) * (P - b), 0);
}
 
 
// Проверка принадлежности углу abc точки p
bool angle(Vect A, Vect B, Vect C, Vect p)
{
	Vect v1 = A - B, v2 = C - B;
	p -= B;
	if ((v1 ^ v2).z > 0)
	{
		swap(v1, v2);
	}
 
	if ((p ^ v2).z <= 0 && (v1 ^ p).z <= 0)
	{
		return true;
	}
	return false;
}
 
 
// Проверка принадлежности многоугольнику точки p
bool polygon(Vect sh, vector < Vect >& p)
{
	int n = p.size();
	if (angle(p[n - 1], p[0], p[1], sh))
	{
		int l = 1, r = n - 1;
		while (r - l > 1)
		{
			int md = (r + l) / 2;
 
			if (angle(p[md], p[0], p[l], sh))
			{
				r = md;
			}
			else
			{
				l = md;
			}
		}
		if (((sh - p[l]) ^ (p[r] - p[l])).z <= 0)
		{
			return true;
		}
	}
	return false;
}
 
 
// Проверка на пересечение двух отрехков
bool check_intersecktion(Vect a, Vect b, Vect c, Vect d)
{
	Line AB(a, b), CD(c, d);
 
	Vect P = AB.intersect(CD);
 
	if (P.x == inf && P.y == inf)
	{
		return AB.check_aff(c) && (cut(a, b, c) || cut(a, b, d) || cut(c, d, a) || cut(c, d, b));
	}
 
	return cut(a, b, P) && cut(c, d, P);
}
 
 
// Кратчайшее расстоянее от точки до отрезка
double dist(Vect a, Vect b, Vect c)
{
	Line AB(a, b);
 
	double csa = (b - a).Сos_angle(c - a), csb = (a - b).Сos_angle(c - b);
	if (csa >= 0 && csb >= 0)
	{
		return abs(AB.N * c + AB.d);
	}
	if (csa >= 0)
	{
		return (c - b).length();
	}
	return (c - a).length();
}
 
 
// Поиск выпуклой оболочки
vector < Vect > get_hull(vector < Vect > p)
{
	int st = -1;
	for (int i = 0; i < p.size(); i++)
	{
		if (st == -1 || p[i].x < p[st].x || p[i].x == p[st].x && p[i].y < p[st].y)
		{
			st = i;
		}
	}
 
	sort(p.begin(), p.end(), [=](Vect a, Vect b)
		{
			return ((b - p[st]) ^ (a - p[st])).z < 0 || ((b - p[st]) ^ (a - p[st])).z == 0 && (a - p[st]).length() >(b - p[st]).length();
		});
 
	int i = 1;
	vector < Vect > hl;
	hl.push_back(p[0]);
	while (i < p.size())
	{
		while (hl.size() > 1 && ((p[i] - hl[hl.size() - 1]) ^ (hl[hl.size() - 1] - hl[hl.size() - 2])).z >= 0)
		{
			hl.pop_back();
		}
		hl.push_back(p[i]);
		i++;
	}
	return hl;
}
 
 
vector < Vect > get_broken_line(vector < Vect > p)
{
	int st = -1;
	for (int i = 0; i < p.size(); i++)
	{
		if (st == -1 || p[i].x < p[st].x || p[i].x == p[st].x && p[i].y < p[st].y)
		{
			st = i;
		}
	}
 
	sort(p.begin(), p.end(), [=](Vect a, Vect b)
		{
			return ((b - p[st]) ^ (a - p[st])).z < 0 || ((b - p[st]) ^ (a - p[st])).z == 0 && (a - p[st]).length() >(b - p[st]).length();
		});
 
	int i = 1;
	vector < Vect > bl;
	bl.push_back(p[0]);
	while (i < p.size() && ((p[0] - p[p.size() - 1]) ^ (p[i] - p[p.size() - 1])).z == 0)
	{
		bl.push_back(p[i]);
		i++;
		if (i == p.size() - 1)
		{
			return {};
		}
	}
	for (int i = 0; i < bl.size() / 2; i++)
	{
		swap(bl[i], bl[bl.size() - 1 - i]);
	}
 
	for (; i < p.size(); i++)
	{
		bl.push_back(p[i]);
	}
 
	return bl;
}
 
 
int main()
{
	ios_base::sync_with_stdio(false); cin.tie(0); cout.tie(0);
	cout.precision(15);
 
	int n, nl = 0;
 
	cin >> n;
 
	vector < Vect > p(n);
	for (int i = 0; i < n; i++)
	{
		cin >> p[i].x >> p[i].y;
		p[i].id = i + 1;
	}
 
	if (n == 2)
	{
		cout << "No solution" << endl;
		return 0;
	}
 
	vector < Vect > bl = get_broken_line(p);
 
	if (bl.size() == 0)
	{
		cout << "No solution" << endl;
		return 0;
	}
 
	for (Vect el : bl)
	{
		cout << el.id << " ";
	}
	cout << endl;
 
	return 0;
}