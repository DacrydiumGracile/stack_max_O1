# Сгенерируем n возможных стеков размера N с числами от 0 до (2^B) - 1 включительно.
# Сперва мы применяем команду push N раз, а затем pop N раз. И таких повторов будет ровно n штук.
# Результаты сохраняем в файл

import os, random
from argparse import ArgumentParser


# Вспомогательные данные и функции
SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))  # i.e. /path/to/dir/
# Расширение файлов с данными
# Возможно стоит взять PRNG, но совсем уж огромных данных мы не планируем генерировать, поэтому скорость генерации не должна быть проблемой

# Работа с переданными аргументами.
def get_parse_args():
  """
  Возвращает объект, содержащий аргументы и опции, переданные при запуске программы.
  """
  parser = ArgumentParser()

  parser.add_argument(
    "-n",
    dest="n",
    metavar="number",
    default=10,
    type=int,
    help="Количество повторных запусков.",
  )

  parser.add_argument(
    "-B",
    dest="B",
    metavar="number",
    default=4,
    type=int,
    help="Количество бит. Указывает, что генерируемые числа не могут быть больше чем 2^B",
  )

  parser.add_argument(
    "-s",
    "--size",
    dest="size",
    metavar="number",
    default=100,
    type=int,
    help="Размер стека.",
  )

  parser.add_argument(
    "-o", "--output",
    dest="output",
    metavar="path",
    help="Файл сохранения. Путь указывается относительно данного скрипта.",
  )

  args = parser.parse_args()

  return args

if __name__ == "__main__":
  Args = get_parse_args()
  n = Args.n
  b = Args.B
  size = Args.size
  if not Args.output:
    raise FileNotFoundError(
      f"Ошибка: Не указан файл, куда сохранить данные см. аргумент -o (--output)")
  file_output_path = os.path.join(SCRIPT_DIR, Args.output)
  with open(file_output_path, "w", encoding="utf-8") as file_output:
    # Записываем в первую строку B и значение для reserve, в нашем случае это --size
    file_output.write(f"{b} {size}\n")
    # Далее сначала size раз push, а потом size раз pop и так повторяем n раз
    for i in range(n):
      for j in range(size):
        file_output.write(f"a {random.randint(0,(2**b)-1)}\n")
      for j in range(size):
        file_output.write(f"p\n")