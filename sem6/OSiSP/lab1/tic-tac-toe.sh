#!/bin/bash

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
current_player=$X
game_over=false

while [[ $game_over == false ]]
do
    clear
    display
    echo "Current player: $current_player"
    read -p "Enter your move (1-9): " field
    moveTo "$field" "$current_player"
    if [[ $? == 0 ]] 
    then
        checkIfWin board "$current_player"
        if [[ $? == 0 ]]
        then
            clear
            display
            echo "Player $current_player wins!"
            game_over=true
        else
            checkIfTie board
            if [[ $? == 0 ]] 
            then
                clear
                display
                echo "TIE!"
                game_over=true
            else
                if [[ $current_player == $X ]] 
                then
                    current_player=$O
                else
                    current_player=$X
                fi
            fi
        fi
    else
        echo "Invalid move. Try again." >&2
        sleep 1
    fi
done