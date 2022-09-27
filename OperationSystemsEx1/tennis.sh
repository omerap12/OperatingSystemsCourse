#!/bin/bash
print_line(){
    echo " --------------------------------- "
}
print_no_ball_line(){
    echo " |       |       #       |       | "
}
print_mid_ball_line(){
    echo " |       |       O       |       | "
}
print_minus_1_ball_line(){
    echo " |       |   O   #       |       | "
}
print_minus_2_ball_line(){
    echo " |   O   |       #       |       | "
}
print_minus_3_ball_line(){
    echo "O|       |       #       |       | "
}
print_plus_1_ball_line(){
   echo " |       |       #   O   |       | "
}
print_plus_2_ball_line(){
    echo " |       |       #       |   O   | "
}
print_plus_3_ball_line(){
    echo " |       |       #       |       |O"
}
print_0_ball_line(){
    echo " |       |       #       |       | "
}
print_scores(){
    echo " Player 1: ${PLAYER1VALUE}         Player 2: ${PLAYER2VALUE} "
}
print_players_moves(){
    echo "       Player 1 played: ${PLAYER1GUESS}"
    echo "       Player 2 played: ${PLAYER2GUESS}"
    echo ""
    echo ""
}
print_player_one_pick_number(){
    echo "PLAYER 1 PICK A NUMBER: "
}
print_player_two_pick_number(){
    echo "PLAYER 2 PICK A NUMBER: "
}
print_not_valid_move(){
    echo "NOT A VALID MOVE !"
}
print_player_one_wins(){
    echo "PLAYER 1 WINS !"
}
print_player_two_wins(){
    echo "PLAYER 2 WINS !"
}
print_draw(){
    echo "IT'S A DRAW !"
}
print_board(){
    print_scores
    print_line
    print_no_ball_line
    print_no_ball_line
    case $STATUS in
    0)    
    print_mid_ball_line
    ;;

    -1)
    print_minus_1_ball_line
    ;;

    -2)
    print_minus_2_ball_line
    ;;

    -3)
    print_minus_3_ball_line
    ;;

    1)
    print_plus_1_ball_line
    ;;

    2)
    print_plus_2_ball_line
    ;;

    3)
    print_plus_3_ball_line
    ;;
    
    *)
    echo -n "unknown"
    ;;
esac
    print_no_ball_line
    print_no_ball_line
    print_line
}

player_one_pick(){
    VALID=1
    while [[ $VALID -eq 1 ]]
    print_player_one_pick_number
    do
        read -s PLAYER1GUESS
        if [[ $PLAYER1GUESS =~ ^[0-9]+$ ]] && [[ $PLAYER1VALUE -ge $PLAYER1GUESS ]]; then
        PLAYER1VALUE=$[$PLAYER1VALUE-$PLAYER1GUESS]
        VALID=2
        return
        else
            print_not_valid_move
        fi
    done
}

player_two_pick(){
    VALID=1
    while [[ $VALID -eq 1 ]]
    print_player_two_pick_number
    do
        read -s PLAYER2GUESS
        if [[ $PLAYER2GUESS =~ ^[0-9]+$ ]] && [[ $PLAYER2VALUE -ge $PLAYER2GUESS ]]; then
        PLAYER2VALUE=$[$PLAYER2VALUE-$PLAYER2GUESS]
        VALID=2
        return
        else
            print_not_valid_move
        fi
    done
}

check_winner(){
    if [[ $STATUS -eq 3 ]]; then
        print_board
        print_players_moves
        GAME=0
        print_player_one_wins
        return
    fi
    if [[ $STATUS -eq -3 ]]; then
        print_board
        print_players_moves
        GAME=0
        print_player_two_wins
        return
    fi
    if [[ $PLAYER2VALUE -gt 0 ]] && [[ $PLAYER1VALUE -eq 0 ]]; then
        print_board
        print_players_moves
        GAME=0
        print_player_two_wins
        return
    fi
    if [[ $PLAYER2VALUE -eq 0 ]] && [[ $PLAYER1VALUE -gt 0 ]]; then
        print_board
        print_players_moves
        GAME=0
        print_player_one_wins
        return
    fi

    if [[ $PLAYER2VALUE -eq 0 ]] && [[ $PLAYER1VALUE -eq 0 ]] && [[ $STATUS -gt 0 ]]; then
        print_board
        print_players_moves
        GAME=0
        print_player_one_wins
        return
    fi
    if [[ $PLAYER2VALUE -eq 0 ]] && [[ $PLAYER1VALUE -eq 0 ]] && [[ $STATUS -lt 0 ]]; then
        print_board
        print_players_moves
        GAME=0
        print_player_two_wins
        return
    fi
    if [[ $PLAYER2VALUE -eq 0 ]] && [[ $PLAYER1VALUE -eq 0 ]] && [[ $STATUS -eq 0 ]]; then
        print_board
        print_players_moves
        GAME=0
        print_draw
        return
    fi


}
handle_moves(){
    FLAGPLAYERONE=0
    FLAGPLAYERTWO=0
    COUNT1=1

    #player 1 won round
    if [[ $PLAYER1GUESS -gt $PLAYER2GUESS ]] && [[ $STATUS -ge 0 ]]; then
        STATUS=$[$STATUS+$COUNT1]
        FLAGPLAYERONE=1
        return
    fi
    if [[ $PLAYER1GUESS -gt $PLAYER2GUESS ]] && [[ $STATUS -lt 0 ]]; then
        STATUS=1
        FLAGPLAYERONE=1
        return
    fi
    #player 2 won round
    if [[ $PLAYER1GUESS -lt $PLAYER2GUESS ]] && [[ $STATUS -ge 0 ]]; then
        STATUS=-1
        FLAGPLAYERTWO=1
        return
    fi
    if [[ $PLAYER1GUESS -lt $PLAYER2GUESS ]] && [[ $STATUS -lt 0 ]]; then
        STATUS=$[$STATUS-$COUNT1]
        FLAGPLAYERTWO=1
        return
    fi

}
game_loop(){
    first_run=1
    GAME=1
    STATUS=0
    isFirst=1
    while [[ $GAME -eq 1 ]]
    do
        print_board
        if [[ $isFirst -eq 0  ]]; then
            print_players_moves
        fi
        
        player_one_pick
        
        player_two_pick
        handle_moves
        check_winner
        first_run=0
        isFirst=0
    done
}

PLAYER1VALUE=50
PLAYER2VALUE=50
PLAYER1GUESS=0
PLAYER2GUESS=0
game_loop
