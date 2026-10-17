#include <cstring>
#include <fstream>
#include <iostream>

void print_unique(std::ofstream& outfile, unsigned char* buffer, int unique) {
  for (int i = 0; i < unique; ++i) {
    outfile.put(static_cast<char>(buffer[i]));
  }
}

void code(std::ifstream& infile, std::ofstream& outfile) {
  unsigned char buffer[128] = {0};
  int last = infile.get();
  if (last == EOF) {
    return;
  }

  int repeat = 1;
  int unique = 0;
  int ch;
  while (EOF != (ch = infile.get())) {
    if (ch == last) {    // повтор€ющиес€ символы
      if (unique > 0) {  // вывод оставшихс€ уникальных
        outfile.put(static_cast<signed char>(-unique));
        print_unique(outfile, buffer, unique);
        unique = 0;
      }
      ++repeat;

      while (repeat > 129) {
        outfile.put(static_cast<signed char>(127));
        outfile.put(static_cast<char>(last));
        repeat -= 129;
      }
    } else {             // уникальные символы
      if (repeat > 1) {  // вывод оставшихс€ повтор€ющихс€
        outfile.put(static_cast<signed char>(repeat - 2));
        outfile.put(static_cast<char>(last));
        repeat = 1;
      } else {
        buffer[unique] = last;
        ++unique;

        if (unique == 128) {
          outfile.put(static_cast<signed char>(-unique));
          print_unique(outfile, buffer, unique);
          unique = 0;
        }
      }
    }
    last = ch;
  }
  // обработка после конца файла
  if (repeat > 1) {
    outfile.put(static_cast<signed char>(repeat - 2));
    outfile.put(static_cast<char>(last));
  } else {
    if (unique == 128) {
      outfile.put(static_cast<signed char>(-unique));
      print_unique(outfile, buffer, unique);
      unique = 0;
    }
    buffer[unique] = last;
    ++unique;
  }
  if (unique > 0) {
    outfile.put(static_cast<signed char>(-unique));
    print_unique(outfile, buffer, unique);
  }
}

void decode(std::ifstream& infile, std::ofstream& outfile) {
  int elm;
  while (EOF != (elm = infile.get())) {
    signed char ch = static_cast<signed char>(elm);
    if (ch < 0) {  // неповтор€ющиес€ символы
      int leng = -ch;
      for (int i = 0; i < leng; ++i) {
        int x = infile.get();
        outfile.put(static_cast<signed char>(x));
      }
    } else {  // сери€
      int leng = ch + 2;
      int x = infile.get();
      for (int i = 0; i < leng; ++i) {
        outfile.put(static_cast<signed char>(x));
      }
    }
  }
}

int main(int argc, char* argv[]) {
  if (argc == 4) {
    std::ifstream infile(argv[2], std::ios_base::binary);
	if (!infile) {
		std::cout << "Can not open file" << std::endl;
		return 0;
	}

    std::ofstream outfile(argv[3], std::ios_base::binary);
    if (!outfile) {
      std::cout << "Can not open file" << std::endl;
	  infile.close();
      return 0;
    }

    if (strcmp(argv[1], "-e") == 0) {  // кодируем
      code(infile, outfile);
    } else if (strcmp(argv[1], "-d") == 0) {  // декодируем
      decode(infile, outfile);
    }

    infile.close();
    outfile.close();
  }
  return 0;
}