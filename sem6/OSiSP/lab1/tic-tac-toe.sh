#!/bin/bash
screenshotFilePath=$1
echo -e "++++++++++++++++++++++++++\n" >> "$screenshotFilePath"
makeScreenshot() {
    exec 4>&1
    exec 1>>"$screenshotFilePath"
    echo " ${board[0]} | ${board[1]} | ${board[2]} " 
    echo "---+---+---"
    echo " ${board[3]} | ${board[4]} | ${board[5]} " 
    echo "---+---+---"
    echo " ${board[6]} | ${board[7]} | ${board[8]} " 
    echo -e "\n" 
    exec 1>&4
}

board=("1" "2" "3" "4" "5" "6" "7" "8" "9")

BRed='\033[1;31m'
BBlue='\033[1;34m' 
RESET_COLOR='\033[0m'

display() {
    local tmp=()
    for element in "${board[@]}"; do
        if [[ $element == "X" ]]; then
            tmp+=("${BRed}${element}${RESET_COLOR}")
        elif [[ $element == "O" ]]; then
            tmp+=("${BBlue}${element}${RESET_COLOR}") 
        else
            tmp+=("$element")
        fi
    done
    
    echo -e " ${tmp[0]} | ${tmp[1]} | ${tmp[2]} "
    echo -e "---+---+---"
    echo -e " ${tmp[3]} | ${tmp[4]} | ${tmp[5]} "
    echo -e "---+---+---"
    echo -e " ${tmp[6]} | ${tmp[7]} | ${tmp[8]} "
}

checkIfWin() {
    local -n b=$1
    local player=$2

    if [[ ${b[0]} == $player && ${b[1]} == $player && ${b[2]} == $player ]] || 
       [[ ${b[3]} == $player && ${b[4]} == $player && ${b[5]} == $player ]] || 
       [[ ${b[6]} == $player && ${b[7]} == $player && ${b[8]} == $player ]] || 
       [[ ${b[0]} == $player && ${b[3]} == $player && ${b[6]} == $player ]] || 
       [[ ${b[1]} == $player && ${b[4]} == $player && ${b[7]} == $player ]] || 
       [[ ${b[2]} == $player && ${b[5]} == $player && ${b[8]} == $player ]] || 
       [[ ${b[0]} == $player && ${b[4]} == $player && ${b[8]} == $player ]] ||
       [[ ${b[2]} == $player && ${b[4]} == $player && ${b[6]} == $player ]]
    then
        return 0 # win
    fi

    return 1 # no win
}

checkIfTie() {
    local -n b=$1

    for i in "${b[@]}"; do
        if [[ "$i" != $O && "$i" != $X ]]; then
            return 1 # not a tie
        fi
    done

    return 0 # tie
}

moveTo() {
    local position=$1
    local player=$2
    ((position -= 1))
    if [[ "${board[$position]}" != $X && "${board[$position]}" != $O ]]
    then
        board[$position]=$player
        return 0 # valid move
    else
        return 1 # invalid move
    fi
}


X="X"
O="O"
curMarker=$X
gameOver=false

while [[ $gameOver == false ]]
do
    clear
    display
    echo "Current player: $curMarker"
    read -p "Enter your move (1-9): " field # user also can make a screenshot

    if [[ $field == "ss" ]]; then
        makeScreenshot
        continue
    elif [[ $field -gt 9 || $field -lt 1 ]]; then
        echo "Invalid move. Try again." >&2
        sleep 1
        continue
    fi

    moveTo "$field" "$curMarker"
    if [[ $? == 0 ]] 
    then
        checkIfWin board "$curMarker"
        if [[ $? == 0 ]]
        then
            clear
            display
            echo "Player $curMarker wins!"
            gameOver=true
        else
            checkIfTie board
            if [[ $? == 0 ]] 
            then
                clear
                display
                echo "TIE!"
                gameOver=true
            else
                if [[ $curMarker == $X ]] 
                then
                    curMarker=$O
                else
                    curMarker=$X
                fi
            fi
        fi
    else
        echo "Invalid move. Try again." >&2
        sleep 1
    fi
done