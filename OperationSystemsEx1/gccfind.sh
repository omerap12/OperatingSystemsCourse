‫‪!#bin/bash/‬‬
if [[ $# < 2 ]]; then
    echo "Not enough parameters"
fi
if [[ $# == 2 ]]; then
    cd $1
    rm -f *.out
    for i in $(grep -l -w -i -s "\b$2\b" *) #-w for To search for the word in all files in the current directory -i for case sensitive -l  To get only the filenames that match your search
    do
        $(gcc -w $i -o ${i%.*}.out) #-w for not warnings, #${i%.*} to get the filename without the extension
    done
fi
if [[ $# == 3 ]] && [[ $3 == "-r" ]]; then
    for d in $(find $1 -type d)
    do
        cd $d
        rm -f *.out
        for i in $(grep -l -w -i -s "\b$2\b" *) #-w for To search for the word in all files in the current directory -i for case sensitive -l  To get only the filenames that match your search
        do
            $(gcc -w $i -o ${i%.*}.out) #-w for not warnings, #${i%.*} to get the filename without the extension
        done
    done
fi
if [[ $# == 3 ]] && [[ $3 != "-r" ]]; then
    cd $1
    rm -f *.out
    for i in $(grep -l -w -i -s "\b$2\b" *) #-w for To search for the word in all files in the current directory -i for case sensitive -l  To get only the filenames that match your search
    do
        $(gcc -w $i -o ${i%.*}.out) #-w for not warnings, #${i%.*} to get the filename without the extension
    done
fi
