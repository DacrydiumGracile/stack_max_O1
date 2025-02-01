import re
import math

import os, sys, uuid
from argparse import ArgumentParser

# Вспомогательные данные и функции
SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))  # i.e. /path/to/dir/
# Расширение файлов с данными
EXT = ".dat"
# Путь до файла, который будет создан или перезаписан, при вызове с -d
# в него будут сохранён список всех файлов, не прошедших проверку.
# Он будет полезен при вызове с -r
Invalid_files_path = os.path.abspath(os.path.join(SCRIPT_DIR, "invalid_files.txt"))   

class bcolors:
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'

def eprint(*args, **kwargs):
  print(f"{bcolors.FAIL}ERROR: ", *args, file=sys.stderr, **kwargs, end=f"\n{bcolors.ENDC}")

def wprint(*args, **kwargs):
  print(f"{bcolors.WARNING}WARNING: ", *args, **kwargs, end=f"\n{bcolors.ENDC}")

def iprint(*args, **kwargs):
  print(f"{bcolors.OKGREEN}INFO:", *args, **kwargs, end=f"\n{bcolors.ENDC}")


# Работа с переданными аргументами.
def get_parse_args():
  """
  Возвращает объект, содержащий аргументы и опции, переданные при запуске программы.
  """
  parser = ArgumentParser()

  parser.add_argument(
    "-d",
    "--dir",
    dest="dir",
    metavar="path",
    help="Проверить все файлы из указанной директории. Путь указывается относительно данного скрипта.",
  )

  parser.add_argument(
    "-f",
    "--file",
    dest="verifiable_file",
    metavar="path",
    help="Проверить указанный файл. Путь указывается относительно данного скрипта.",
  )

  parser.add_argument(
    "-r",
    dest="files_list",
    metavar="path",
    help="Проверить файлы, пути к которым указаны в path-файле. Путь указывается относительно данного скрипта.",
  )

  args = parser.parse_args()

  return args


class InvalidDataError(Exception):
  def __init__(self, message, line_number=None):
    self.message = message
    self.line_number = line_number
    super().__init__(message)


def check_data(file_path):
  with open(file_path, "r", encoding="utf-8") as f:
    # Проверка первой строки
    first_line = f.readline()
    match = re.match(r"^(\d+)\s+(\d+)$", first_line)
    if not match:
      raise InvalidDataError(
        "Первая строка должна содержать два целых неотрицательных числа", 1
      )
    try:
      B, max_number_elements = map(int, match.groups())
    except ValueError:
      raise InvalidDataError("Значения должны быть целыми числами.", 1)
    if B < 2:
      raise InvalidDataError("Первое число должно быть не меньше 2.", 1)

    # Инициализация счетчиков
    count_a = 0
    count_p = 0
    max_diff = 0

    # Проверка остальных строк
    for i, line in enumerate(f, start=2):
      match = re.match(r"^a\s+(\d+)\s*$", line)
      if match:
        try:
          x = int(match.group(1))
        except ValueError:
          raise InvalidDataError("Значение должно быть целым числом.", i)
        if not 0 <= x <= (2**B - 1):
          raise InvalidDataError(
            f"Значение '{x}' выходит за пределы допустимого диапазона [0, {2**B-1}].",
            i,
          )
        count_a += 1
      else:
        match = re.match(r"^p\s*$", line)
        if match:
          count_p += 1
        else:
          raise InvalidDataError(
            "Строка должна иметь вид 'a' число или или быть равной 'p'.", i
          )

      # Обновление максимальной разницы
      diff = count_a - count_p
      max_diff = max(max_diff, diff)

      # Проверка дополнительных ограничений
      if count_a < count_p:
        raise InvalidDataError(
          "Количество строк с 'a' должно быть не меньше количества строк с 'p'.",
          i,
        )

    # Проверка второго числа
    if max_number_elements < max_diff:
      raise InvalidDataError(f"Второе число должно быть не меньше {max_diff}.", 1)
    # Рекомендация для выбора оптимального второго числа
    # Рекомендация изменить второе число
    if max_number_elements > max_diff + 1:
      recommended_max_number_elements = math.ceil(max_diff / 2) * 2
      wprint(f"Тестовые данные файла '{file_path}' корректны, но рекомендуется изменить второе число на {recommended_max_number_elements} для большей производительности.")
    else:
      iprint(f"Тестовые данные файла '{file_path}' корректны.")


def check_file(file_path):
  try:
    # iprint(f"Проверяется файл '{file_path}'.")
    check_data(file_path)
    return None
  except InvalidDataError as e:
    eprint(f"Тестовые данные не корректны: {e.message} (строка № {e.line_number}).")
    return file_path


if __name__ == "__main__":
  Args = get_parse_args()
  if Args.dir:
    dir_path = os.path.abspath(os.path.join(SCRIPT_DIR, Args.dir))
    if not os.path.exists(dir_path) or not os.path.isdir(dir_path):
      raise FileNotFoundError(
        f"Ошибка: Директория '{dir_path}' отсутствует. Убедитесь в её наличии."
      )

    # Создадим файл, в которым запишем все провальные файлы
    with open(Invalid_files_path, "w", encoding="utf-8") as log_file:
      # проверяем все файлы с расширением из EXT из этой директории
      with os.scandir(dir_path) as it:
        for entry in it:
          file_path = entry.path
          if (
            file_path.startswith(".")
            or not entry.is_file()
            or not file_path.endswith(EXT)
          ):
            continue
          if check_file(file_path):
            # Файл не прошёл проверку, поэтому записываем его в файл, для повторного выполнения.
            log_file.write(file_path + "\n")
          # файл удовлетворяет требованиям.
  
  elif Args.verifiable_file:
    verifiable_file_path = os.path.abspath(
      os.path.join(SCRIPT_DIR, Args.verifiable_file)
    )
    if not os.path.exists(verifiable_file_path):
      raise FileNotFoundError(
        f"Ошибка: Файл '{verifiable_file_path}' отсутствует. Убедитесь в наличие данного файла."
      )
    check_file(verifiable_file_path)
    
  elif Args.files_list:
    files_list_path = os.path.abspath(os.path.join(SCRIPT_DIR, Args.files_list))
    new_file_path = os.path.abspath(os.path.join(SCRIPT_DIR, str(uuid.uuid4())))
    if not os.path.exists(files_list_path):
      raise FileNotFoundError(
        f"Ошибка: Файл '{files_list_path}' отсутствует. Убедитесь в наличие данного файла."
      )
    with open(files_list_path, "r", encoding="utf-8") as file_input:
      with open(new_file_path, "w") as file_output:
        for file_path in file_input:
          verifiable_file_path = os.path.abspath(file_path.strip())
          iprint(repr(verifiable_file_path))
          
          if not os.path.exists(verifiable_file_path):
            raise FileNotFoundError(
              f"Ошибка: Неверно указаны данные в файле '{files_list_path}'.\nФайл '{verifiable_file_path}' отсутствует. Убедитесь в наличие данного файла."
            )
          if check_file(verifiable_file_path):
            file_output.write(verifiable_file_path + "\n")
    # Переименует и заменит исходный файл на новый файл, в котором будет либо меньше невалидных файлов, либо столько же.
    os.replace(new_file_path, files_list_path)