# Algorithms

## Usage

0. Requirements:

   - OS: unix like
   - cmake >= `3.20`
   - gcc >= `11.0.0`
   - xclip

1. Clone repository:

   ```bash
   git clone https://github.com/GrigoriyPA/OlympAlgorithms.git
   cd OlympAlgorithms
   ```

2. Create working directory `dev/` and use one of templates:

   ```bash
   mkdir dev/
   cp templates/CMakeLists.txt dev/
   cp templates/leet_code/* dev/
   ```

3. Write solution in `dev/solution.h`
4. Expand includes:

   ```bash
   ./dev/prepare.sh
   ```

5. Submit file `dev/submit.h`
