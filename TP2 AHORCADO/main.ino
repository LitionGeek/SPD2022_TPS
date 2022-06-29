// C++ code
//

// DEFINES //
#define BUTTON_RIGHT 8
#define BUTTON_SELECT 9
#define BUTTON_LEFT 10
#define BUTTON_CHEAT 11
#define NUM 5
#define TIME 2000
#include <LiquidCrystal.h>

LiquidCrystal lcd(2, 3, 4, 5, 6, 7);

void setup()
{
    lcd.begin(16, 2);
    pinMode(BUTTON_RIGHT, INPUT);
    pinMode(BUTTON_SELECT, INPUT);
    pinMode(BUTTON_LEFT, INPUT);
    pinMode(BUTTON_CHEAT, INPUT);
    randomSeed(analogRead(0));
}

// DECLARACION DE VARIABLES //
unsigned long millis_before = 0;
int lettersCounter = 0;
int wordSize;
short button_cheat_before = LOW;
short button_left_before = LOW;
short button_right_before = LOW;
short button_select_before = LOW;
char guessLetter;
short cheat;
int cheatPos = 0;
short cheatSize;
short correctLetter;
short flagTitle = 0;
short gameLost = 0;
short gameOver;
short gameStarted = 0;
short gameWin;
short guessedCount = 0;
short i;
short infiniteLives = 0;
short level = 1;
short lifePoints;
short lettersPut;
short nextAction;
short selected;
short showWord;
short totalRight;
short totalCheatRight;
short x;
char cheatMK[] = {"ABACABB"};
char cheatKC[] = {"UUDDLRLRBAS"};
char cheatWord[13];
char guessedLetter[800];
char guessWord[13];
char letters[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
char *words1[] = {"HOMERO", "MARGE", "BART", "LISA", "MAGGIE"};
char *words2[] = {"MARTY", "EMMET", "JENNIFER", "GEORGE", "BIFF"};
char *words3[] = {"ALPHAMON", "OMEGAMON", "MAGNAMON", "DUKEMON", "YGGDRASILL"};
char *words4[] = {"THOR", "IRONMAN", "WASP", "ANTMAN", "HULK"};
char *words5[] = {"VALKYR", "MESA", "EXCALIBUR", "SEVAGOTH", "OCTAVIA"};

/**
 * @brief Imprime un espacio en la posicion indicada.
 * 
 * @param col Columna donde imprime el espacio.
 * @param row Fila donde imprime el espacio.
 */
void clearCR(int col, int row)
{
    lcd.setCursor(col, row);
    lcd.print(" ");
}
/**
 * @brief Imprime un espacio en todas las columnas de una fila.
 * 
 * @param row Fila que va a imprimir un espacio,
 */
void clear_col(int row)
{
    for (i = 0; i < 16; i++)
    {
        clearCR(i, row);
    }
}

/**
 * @brief Imprime un espacio en las dos filas de la pantalla
 * llamando dos veces a la funcion clear_col
 * 
 */
void clear_screen()
{
    clear_col(0);
    clear_col(1);
}

/**
 * @brief Genera una palabra al azardependiendo del nivel
 * y la copia en la variable guessWord.
 *  
 */
void new_word()
{
    int pick = random(NUM);
    switch (level)
    {
        case 1:
            strcpy(guessWord, words1[pick]);
        break;

        case 2:
            strcpy(guessWord, words2[pick]);
        break;

        case 3:
            strcpy(guessWord, words3[pick]);
        break;

        case 4:
            strcpy(guessWord, words4[pick]);
        break;

        case 5:
            strcpy(guessWord, words5[pick]);
        break;
    }
}

/**
 * @brief Llama a las fucniones necesarias y deja todo listo para iniciar el juego.
 * Llamma a la funcion para generar una nueva palabra y calcula el tamaño.
 * Asigna los puntos de vida.
 * Llama a la funcion para mostrar los guiones, 
 * Pone las banderas en 0.
 * Ademas pone NULL en guessLetter para que no muestre basura al empezar y 
 * llena de "a" guessedLetter para reemplazar las letras en caso de que se superen los niveles.
 * Finalmente pone la bandera para iniciar el juego en 1.
 * 
 */
void game_start()
{
    new_word();
    wordSize = strlen(guessWord);
    set_life_points();
    show_blanks();
    totalRight = 0;
    lettersCounter = 0;
    guessedCount = 0;
    guessLetter = NULL;
    memset(guessedLetter, 'a', strlen(guessedLetter));
    gameStarted = 1;
}

/**
 * @brief Si el nivel es 1, asigna los puntos de vida en 3.
 * 
 */
void set_life_points()
{
    if (level == 1)
    {
        lifePoints = 3;
    }
}
/**
 * @brief Imprime en la pantalla LCD el titulo del juego si no se mostro todavia.
 * Si se mostro pone la flagTitle en 1 y pone nextAction en 1.
 * 
 */
void show_title()
{
    if(flagTitle == 0)
    {
        lcd.setCursor(4, 0);
        lcd.print("AHORCADO");
        nextAction = 1;
        flagTitle = 1;
    } 
}

/**
 * @brief Imprime en la pantalla LCD el nivel.
 * 
 */
void show_level()
{
    lcd.setCursor(4, 0);
    lcd.print("NIVEL ");
    lcd.print(level);
}

/**
 * @brief Imprime en la pantalla LCD un guion bajo por cada letra 
 * que tenga la palabra.
 * 
 */
void show_blanks()
{
    lcd.setCursor(3, 1);
    for (i = 0; i < wordSize; i++)
    {
        lcd.print("_");
    }
}

/**
 * @brief Imprime en la pantalla LCD los puntos de vida y se va actualizando si se pierden.
 * 
 */
void show_life_points()
{
    lcd.setCursor(10, 0);
    lcd.print("LP:");
    switch (lifePoints)
    {
    case 3:
        lcd.print("***");
        break;

    case 2:
        lcd.print("**");
        clearCR(15, 0);
        break;

    case 1:
        lcd.print("*");
        clearCR(14, 0);
        break;

    case 0:
        clearCR(13, 0);
        break;
    }
}

/**
 * @brief Imprime en la pantalla LCD la letra en la que se esta parado.}
 * con dos felchas que la señalan.
 * 
 */
void show_letter()
{
    lcd.setCursor(4, 0);
    lcd.print(">");
    lcd.print(letters[lettersCounter]);
    lcd.print("<");
}

/**
 * @brief Imprime en la pantalla LCD las dos letras anteriores a la 
 * izquierda de la letra que se esta parado.
 * 
 */
void show_leters_left()
{
    lcd.setCursor(2, 0);
    for (i = 2; i >= 1; i--)
    {
        if (lettersCounter - i >= 0)
        {
            lcd.print(letters[lettersCounter - i]);
        }
        else
        {
            if (lettersCounter == 0)
            {
                lcd.print(letters[24]);
                lcd.print(letters[25]);
                break;
            }
            else
            {
                lcd.print(letters[25]);
            }
        }
    }
}

/**
 * @brief Imprime en la pantalla LCD las dos letras posteriores a la 
 * derecha de la letra que se esta parado.
 * 
 */
void show_letters_right()
{
    lcd.setCursor(7, 0);
    for (i = 1; i <= 2; i++)
    {
        if (lettersCounter + i <= 25)
        {
            lcd.print(letters[lettersCounter + i]);
        }
        else
        {
            if (lettersCounter == 25)
            {
                lcd.print(letters[0]);
                lcd.print(letters[1]);
                break;
            }
            else
            {
                lcd.print(letters[0]);
            }
        }
    }
}

/**
 * @brief Si la letra seleccionada no es NULL la imprime en la pantalla del LCD.
 * 
 */
void show_last_letter()
{
    if (guessLetter != NULL)
    {
        lcd.setCursor(0, 0);
        lcd.print(guessLetter);
    }
}

/**
 * @brief Imprime por la pantalla LCD "YOU WIN"
 * y la palabra que habia que adivinar.
 * 
 */
void show_winner()
{
    lcd.setCursor(4, 0);
    lcd.print("YOU WIN");
    lcd.setCursor(4, 1);
    lcd.print(guessWord);
}

/**
 * @brief Imprime por la pantalla LCD "GAME OVER".
 * 
 */
void show_game_over()
{
    lcd.setCursor(6, 0);
    lcd.print("GAME");
    lcd.setCursor(6, 1);
    lcd.print("OVER");
}

/**
 * @brief Imprime por la pantalla LCD "CHAMPION".
 * 
 */
void show_champion()
{
    lcd.setCursor(4, 1);
    lcd.print("CHAMPION");
}

/**
 * @brief Si el modo cheat esta activado imprime "CH", si no lo borrra
 * 
 */
void show_cheat_mode()
{
    if (cheat == 1)
    {
        lcd.setCursor(14, 1);
        lcd.print("CM");
    }
    else
    {
        clearCR(14, 1);
        clearCR(15, 1);
    }
}

/**
 * @brief Decrementa el contador de letras en uno,
 * si baja de 0 lo devuelve a 25.
 * 
 */
void letters_counter_down()
{
    lettersCounter--;
    if (lettersCounter < 0)
    {
        lettersCounter = 25;
    }
}

/**
 * @brief Incrementa el contador de letras en uno,
 * si sube de 25 lo vuelve a 0.
 * 
 */
void letters_counter_up()
{
    lettersCounter++;
    if (lettersCounter > 25)
    {
        lettersCounter = 0;
    }
}

/**
 * @brief Guarda la letra en la posicion en la que se encuentra el contador.
 * pone la bandera selected en 1 para indicar que se selecciono una letra.
 * 
 */
void select_Letter()
{
    guessLetter = letters[lettersCounter];
    selected = 1;
}  

/**
 * @brief Recorre el la palabra a adivinar, 
 * si la letra seleccionada coincide con una o mas letras las imprime en la pantalla LCD y cuenta cuantas imprimio.
 * 
 * @param letter 
 */
void show_correct_letters(char letter)
{   
    lettersPut = 0;
    for (i = 0; i < wordSize; i++)
    {
        if (letter == guessWord[i])
        {
            lcd.setCursor(i + 3, 1);
            lcd.print(letter);
            lettersPut++;
        }
    }
}

/**
 * @brief Si las letras hay letras impresas mayor a 0 y las letreas no fueron ya adivinadas
 * copia la cantidad de letras que se imprimio, las gurarda en el total de aciertos,
 * y pone correctLetter en 1.
 * Si no pone correctLetter en 0.
 * 
 */
void close_to_victtory()
{
    if(lettersPut > 0 && !already_guessed())
    {
        totalRight += lettersPut;
        correctLetter = 1;
    }
    else
    {
        correctLetter = 0;
    }
}

/**
 * @brief Agrega la letra seleccionada al array de letras acertadas.
 * 
 */
void guessed_letters()
{
    guessedLetter[guessedCount] = guessLetter;
    guessedCount++;
}

/**
 * @brief Recorre las letras que fueron adivinadas, 
 * y revisa que la letra selecionada esta o no en el array.
 * 
 * @return int Retorna 0 si la letra seleccionada no esta y 1 si esta. 
 */
int already_guessed()
{
    int retorno;
    int size;

    retorno = 0;
    size = strlen(guessedLetter);

    for (i = 0; i < size; i++)
    {
        if (guessLetter == guessedLetter[i])
        {
            retorno = 1;
            break;
        }
    }
    return retorno;
}


/**
 * @brief Si el modo cheat esta desactivado, lo activa, si no, lo desactiva.
 * 
 */
void cheat_on_off()
{
    if (cheat != 1)
    {
        cheat = 1;
    }
    else
    {
        cheat = 0;
    }
}

/**
 * @brief Reimprime en la pantalla LCD las letras ya acertadas.
 * 
 */
void rewrite_letters()
{
    int size;

    size = strlen(guessedLetter);

    for (x = 0; x < size; x++)
    {
        show_correct_letters(guessedLetter[x]);
    }
}

/**
 * @brief Restaura los guines bajos y las letras que fueron acertadas.
 *  * 
 */
void restore()
{
    show_blanks();
    rewrite_letters();
}

void need_restore()
{
    if (cheat == 0 && showWord == 1)
    {
        restore();
        showWord = 0;
    }
}

/**
 * @brief Si la letra seleccionada coincide con la letra del truco en la posicion indicada,
 *  la guarda en un array e incrementa la posicion en 1 para comparar la siguiente letra.
 * 
 */
void cheats()
{
    if (guessLetter == cheatMK[cheatPos] || guessLetter == cheatKC[cheatPos])
    {
        cheatWord[cheatPos] = guessLetter;
        cheatPos++;
    }
}

/**
 * @brief Truco de Mortal Kombat que muestra la sancgre... aca muestra la palabra a adivinar.
 * Siempre y cuando las letras en el array que guardo cheat coincidan con "ABACABB".
 */
void cheats_mortal_kombat()
{
    if (strcmp(cheatWord, cheatMK) == 0)
    {
        lcd.setCursor(3, 1);
        lcd.print(guessWord);
        showWord = 1;
        cheatPos = 0;
    }
}

/**
 * @brief Clasico codigo Konami, da vidas infinitas.
 *Siempre y cuando las letras en el array que guardo cheat coincidan con "UUDDLRLRBAS".
 */
void chet_konami_code()
{
    if (strcmp(cheatWord, cheatKC) == 0)
    {
        infiniteLives = 1;
        cheatPos = 0;
    }
}

/**
 * @brief Antirebote del boton cheat.
 * Llama a la funcion para activar los trucos.
 * y a la funcion que analiza si necesita restaurar las letras.
 * Pone guessLetter en NULL para que no lo tome como error y descuente una vida.
 * 
 */
void press_button_cheat()
{
    int button_cheat_now = digitalRead(BUTTON_CHEAT);
    if (button_cheat_now == HIGH && button_cheat_before == LOW)
    {
        cheat_on_off();
        need_restore();
        guessLetter = NULL;
    }
    button_cheat_before = button_cheat_now;
}

/**
 * @brief Antirebote del bboton izquierdo.
 * Llama a la funcion que hace decrementar el contador de letras.
 * 
 */
void press_button_left()
{
    int button_left_now = digitalRead(BUTTON_LEFT);
    if (button_left_now == HIGH && button_left_before == LOW)
    {
        letters_counter_down();
    }
    button_left_before = button_left_now;
}

/**
 * @brief Antirebote del boton derecho.
 * Llama a la funcion que hace incrementar el contador de letras.
 * 
 */
void press_button_right()
{
    int button_right_now = digitalRead(BUTTON_RIGHT);
    if (button_right_now == HIGH && button_right_before == LOW)
    {
        letters_counter_up();
    }
    button_right_before = button_right_now;
}

/**
 * @brief Antirebote del boton select.
 * Llama a la fncion que seleciona.
 * Si el modo cheat no esta activado llama a las funciones que analizan si la letra selecionada
 * esta en la palabra a adivinar.
 * Si el modo cheat esta en 1 llama a las funciones que dejan ingresar los trucos.
 */
void press_button_select()
{
    int button_select_now = digitalRead(BUTTON_SELECT);
    if (button_select_now == HIGH && button_select_before == LOW)
    {
        select_Letter();
        if (cheat == 0)
        {
            show_correct_letters(guessLetter);
            close_to_victtory();
            if (correctLetter == 1)
            {
                guessed_letters();
            }
        }
        else
        {
            cheats();
            cheats_mortal_kombat();
            chet_konami_code();
        }   
    }
    button_select_before = button_select_now;
}

/**
 * @brief Lista de acciones a realizar.
 * Si esta en 1: Limpia la pantalla y muestra el nivel, pone nextAction en 2,
 * Si esta en 2: Limpia la pantalla y llama a la funcion para iniciar el juego, pone nextAction en 0.
 * Si esta en 3: Limpia la pantalla y muestra game over y pone nextAction en 1 y level en 1 para reiniciar el juego.
 * Si esta en 4: Limpia la pantalla y muestra winner, si el nivel es menor a 5 lo incrementa y póne a nextAction como 1, sino elige 5 como nextAction.
 * Si esta en 5: Limpia la pantalla, mutra el titulo y abajo champeon, despues pone nextAction en 1 para reiniciar el juego desde el primer nivel
 */
void actions()
{
    switch (nextAction)
    {
        case 1:
            clear_screen();
            show_level();
            nextAction = 2;
        break;
    
        case 2:
            clear_screen();
            game_start();
            nextAction = 0;
        break;

        case 3:
            clear_screen();
            show_game_over();
            nextAction = 1;
            level = 1;
        break;

        case 4:
            clear_screen();
            show_winner();
            if (level < 5)
            {
                level++;
                nextAction = 1;
            }
            else
            {
                nextAction = 5;
            }
        break;

        case 5:
            clear_screen();
            flagTitle = 0;
            show_title();
            show_champion();
            nextAction = 1;
            level = 1;
        break;
    }
}

/**
 * @brief secuencia que ejecuta una accion cada 2 segundos.
 * 
 */
void sequence()
{
    unsigned long millis_now = millis();
    if (millis_now - millis_before >= TIME)
    {
        actions();

        millis_before = millis_now;
    }
}

void loop()
{
    show_title();
    sequence();
    //Si el juego empezo se pueden usar los botones.
    //y se muestra la vida.
    if (gameStarted == 1)
    {
        show_life_points();
        press_button_left();
        press_button_select();
        press_button_right();
        show_letters_right();
        show_letter();
        show_leters_left();
        show_last_letter();
        press_button_cheat();
        show_cheat_mode();

        //Se pierde una vida si se selecciono una,
        //no se acerto a la correcta, el cheat mode esta desactivado,
        //y no hay vidas infinitas.
        if (selected == 1 &&
            correctLetter == 0 &&
            cheat == 0 &&
            guessLetter != NULL &&
            infiniteLives == 0)
        {
            lifePoints--;
            //Si las vidas llegan a 0 se ejecuta el case 3 de las acciones.
            //game started en 0 para interrumpir el uso de botones y las funciones que muestran.
            if (lifePoints == 0)
            {
                gameStarted = 0;
                nextAction = 3;
            }
        }
        else
        {
            //Si la cantidad de letras correctas es igual a la cantidad de letras de la palabra.
            //se ejecuta el case 4 de las acciones.
            //game started en 0 para interrumpir el uso de botones y las funciones que muestran.
            if (totalRight == wordSize)
            {
                gameStarted = 0;
                nextAction = 4;
            }
        
        }
        
        selected = 0;
    
    }
    
    delay(80);
}
