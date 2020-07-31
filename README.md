# Morse Decoder
Простое приложение направленное на генерацию валидных последовательностей кода морзе из  файла, содержащего строки, состоящие из "." и "-" без пробелов.
Также, присутствует серверная часть приложения, направленная на визуализацию получившихся последовательностей в виде остовного дерева. Дерево задается из графа,
который строится по правилу: Вершины представляют последовательности; ребра между вершинами строятся только при условии,
что между последовательностями, соответствующих ранее упомянутым вершинам, разница составляет в один символ пробела.

## Требования
```
sudo apt-get update && sudo apt-get install qt5-default
```

## Установка
```sh
git clone https://github.com/Vinceent/morse_decoder
cd morse_decoder
sh installation.sh
```
После завершения installation.sh в дирректории появятся 4 новых файла:
* **morse_decoder** - Программа, генерирующая валидные последовательности. Также она создает граф и остовное дерево, которое, при нажатии кнопки "Визуализировать", 
передает для визуализации в **graphic_visualization** . Визуализации подвергаются только строки длинной менее 6 символов.
* **graphic_visualization** - Программа визуализации, получает переданные от **morse_decoder** данные, формирует списки для выбора последовательности, 
а также визуализирует остовное дерево. Способна асинхронно обрабатывать подключения нескольких запущенных **morse_decoder** .
* **morse** - Вспомогательный файл для генерации таблицы морзе в **morse_decoder** . При отсутсвии этого файла в дирректории будет использована 
стандартная таблица кодов морзе.
* **test_code** - Тестовый файл ввода с готовым набором последовательностей.

# Автор
    Artem Kytkin, ETU, 2020
