if [[ $1 == "" ]]
    then 
    echo "Usage: run.sh <filename>"
else
    echo "Compiling $1"
    g++ $1.cpp -lGLEW -lGL -lGLU -lglut -lSOIL -o $1
    echo "Running $1"
    ./$1
fi