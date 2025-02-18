# PermanentSet
Реализация бинарного дерева, хранящего свою предыдущую версию.
# Клонирование репозитория
```
git clone https://github.com/kakov1/PermanentSet.git
cd PermanentSet/
```
# Настройка виртуального окружения
```
python3 -m venv .venv
source .venv/bin/activate
pip3 install conan
conan profile detect --force
```
# Сборка проекта
```
conan install . --output-folder=build --build=missing
cd build/
cmake .. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
cmake --build .
```
# Запуск основной программы
```
cd src/
./main
```
# Запуск тестов
```
cd tests/
./test
```
