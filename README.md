# Ozozo
Проект для программирования Ozo-ботов (далее озоботов) в соответствии с инструкциями log-файлов алгоритма AA-SIPP-m
## Класс OzoBot
Класс OzoBot позволяет хранить информацию об отдельном озоботе:
<li> int Sx - координата X начальной позиции</li>
<li> int Sy - координата Y начальной позиции</li>
<li> int Shead - угловой поворот в стартовой позиции (0 - вдоль оси Ox)</li>
<li> int Gx - координата X целевой позиции</li>
<li> int Gy - координата Y целевой позиции</li>
<li> int Shead - угловой поворот в стартовой позиции (0 - вдоль оси Ox)</li>
<li> float moveSpeed - скорость движения озобота (относительно длины стороны одного поля, 1.0 - одно поле в секунду)</li>
<li> float rotateSpeed - скорость поворота озобота на месте (относительно поворота на 180 градусов, 1.0 - поворот на 180 грудусов за секунду)</li>
<li> OzoColor color - цвет подсветки верхнего и крайних передних светодиодов озобота в виде структуры OzoColor</li>
<li> string compiledColorCode - строка из обзначений цветов для программиррования озобота</li>

Класс имеет конструктор, в который передаются все параметры, из файла логов:

OzoBot(int Sx = 0, int Sy = 0, int Shead = 0, int Gx = 0, int Gy = 0, int Ghead = 0, int moveSpeed = 0, int rotateSpeed = 0, OzoColor color = OzoColor());

Также в последним аргументом коструктора является экземпляр структуры OzoColor, содержащий три целых числа в диапазоне [0..127], описывающих RGB цвет; ее пустой конструктор вернет случайный цвет

Класс OzoBot имеет два ключевых метода:

void appendSection(int type, int parameter)

  Добавляет секцию движения озобота к уже существующей последовательности алгоритма
  
  Имеет аргумента type в который передается одна из констант описанных в файле OzoBot.h:
  
  1)OzoStay - заставляет озобота стоять на месте, не двигаясь время, переданное в parameter в сотых долях секунды
  
  2)OzoMove - заставляет озобота передвинутся на расстоние, переданное в parameter в мм, со скоростью moveSpeed
  
  3)OzoRotate - заставляет озобота повернутся на угол, переданный в parameter в градусах, со скоростью rotateSpeed (положительный угол повернет озобота против часовой стрелки, отрицательный - по часовой)
  
string compColorCode()

  Компилирует и обнуляет запомненную последовательность алгоритма движения озобота в строку string (возвращая ее и записывая в compiledColorCode), каждый символ которой представляет собой обозначение цвета:
  
  1)'K' - черный цвет
  
  2)'R' - красный цвет
  
  3)'G' - зеленый цвет
  
  4)'Y' - желтый цвет
  
  5)'B' - синий цвет
  
  6)'M' - фиолетовый цвет (маджента)
  
  7)'C' - голубой цвет
  
  8)'W' - белый цвет
