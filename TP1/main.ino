
/*
 *	Trabajo Práctico 1: Cronómetro Binario
 *
 *	Al iniciar la simulacion, todos los led deben comenzar apagados.
 *	Cada 1 segundo que transcurra, se deben encender los led
 *	de acuerdo al numero que representan.
 *
 *	Ejemplo:
 *
 *	El numero 15 (0000001111) se representaria encendiendo los
 *	leds AMARILLO y NARANJA.
 *	Al pasar al numero 16 (0000010000), se deben apagar todos
 *	los led anteriores y encenderse uno de los leds VERDES.
 *	Notese, en el ejemplo los 0 representan los led apagados
 *	y los 1 los led encendidos).
 *
 *	-------------------------------------------------------
 *
 *	Al presionarse el boton START, debe iniciar el cronometro.
 *	Volver a presionarlo hace que la secuencia se detenga.
 *  (Como si se hubiese presionado pausa).
 *	Al presionarse el boton RESET, el cronometro
 *	debe reiniciarse y arrancar de 0.
 *
 *	Tip: Modularizar la función que controla el encendido de los
 *	LEDS y de ser posible, todo el código para evitar repetir lineas lo mas posible.
 *  Usar millis para controlar el tiempo del contador para que el cambio de los
 *	leds encendidos sea perceptible para el ojo humano y
 *	documentar cada función creada en el código.
 *	Un breve comentario que diga que es lo que hace esa función
 *  y de corresponder, que retorna).
 */

//##### Una Ayudita #####
//--- Defines ---//
#define FIRST_LED 4 // Pin del rimer Led.
#define LAST_LED 13 // Pin del Ultimo Led.
#define TIME 1000   // Cada cuanto tiempo se ejecuta la sequiencia.

#define TAM 13 // Tamaño del array
/* IMPORTANTE: El array tiene espacios de mas porque porque si no en la simulacion el boton de inicio no funciona bien,
y tnia que mantenerlo presionado para que funciones. ademas que no me muestra el meme.
Le di bastantes vueltas pero no llego a entender que relacion hay para que funcione asi.
Igualmente, aunque el array sea de 13, el cornometo no va a contar mas de 1023.
*/

#define BUTTON_START 2   // Pin del boton start.
#define BUTTON_RESET 3   // Pin del boton reset.
#define MAX_SECONDS 1023 // Limite del contador.
#define ZERO_TO_HERO 0   // Inicio del contador, y para el resey tambien.
//--- End Defines ---//

void setup()
{
    for (int led = FIRST_LED; led <= LAST_LED; led++)
    {
        pinMode(led, OUTPUT);
    }
    Serial.begin(9600);
}

/** variables **/
short contador = ZERO_TO_HERO;
short i;
unsigned long millis_before = 0;
int arrayEnteros[TAM];
short button_start_before = LOW;
short button_reset_before = LOW;
short habilitar = LOW;
short flagMeme = 0;
short flagTitulo = 0;
short flagReset = 0;
short flagStart = 0;
/**
 * @brief Apaga un led.
 *
 * @param led El led que va a apagar.
 */
void shutdown_leds(int led)
{
    digitalWrite(i, LOW);
}

/**
 * @brief Prende un led.
 *
 * @param led El ed que va a prender.
 */
void show_leds(int led)
{
    digitalWrite(led, HIGH);
}

/**
 * @brief Transforma el numero que recibe por el contador en binario y lo guarda
 * en un array.
 *
 * @param cont El contador con el numero a convertir en binario.
 * @param array El array donde va guardar el numero en binario.
 */
void binary(int cont, int array[])
{
    for (i = 0; i <= TAM; i++)
    {
        array[i] = 0;
    }

    for (i = FIRST_LED; i <= LAST_LED; i++)
    {
        array[i] = cont % 2;

        cont /= 2;
    }
}

/**
 * @brief Prende o apaga los leds en base a los valores que tenga el array.
 *
 * @param array El array que tiene los valores.
 */

void leds(int array[])
{
    for (i = FIRST_LED; i <= LAST_LED; i++)
    {
        if (array[i] == 0)
        {
            shutdown_leds(i);
        }
        else
        {
            show_leds(i);
        }
    }
}

/**
 * @brief Imprime por consola un titulo re lindo y "START" para inicar que se inicio la sequencia,
 * y solo al inicio de la sequencia.
 *
 */
void print_cronometro_start()
{
    if (flagTitulo == 0)
    {
        Serial.println("   ____                                      _                    ");
        Serial.println("  / ___|_ __ ___  _ __   ___  _ __ ___   ___| |_ _ __ ___         ");
        Serial.println(" | |   | '__/ _ \\| '_ \\ / _ \\| '_ ` _ \\ / _ \\ __| '__/ _ \\  ");
        Serial.println(" | |___| | | (_) | | | | (_) | | | | | |  __/ |_| | | (_) |       ");
        Serial.println("  \\____|_|  \\___/|_| |_|\\___/|_| |_| |_|\\___|\\__|_|  \\___/  ");
        Serial.println();
        Serial.println();
        Serial.print("\t\t     <---- ");
        Serial.print("START");
        Serial.print(" ---->");
        Serial.println("");

        flagTitulo = 1;
    }
}

/**
 * @brief Muestra por consola las horas.
 *
 * @param contador El contador con los segundos que convierte en horas.
 */
void print_horas(int contador)
{
    if (contador >= 3600)
    {
        Serial.print(contador / 3600);
    }
    else
    {
        Serial.print(0);
    }
}

/**
 * @brief Muestra por consola los minutos.
 *
 * @param contador El contador con los segundos que convierte en minutos.
 */
void print_minutos(int contador)
{
    if (contador >= 60)
    {
        if (contador >= 3600)
        {
            Serial.print((contador % 3600) / 60);
        }
        else
        {
            Serial.print(contador / 60);
        }
    }
    else
    {
        Serial.print(0);
    }
}
/**
 * @brief Muestra por consola los segundos.
 *
 * @param contador El contador con los segundos.
 */
void print_segundos(int contador)
{
    if (contador >= 60)
    {
        if (contador > 3600)
        {
            Serial.print((contador % 3600) % 60);
        }
        else
        {
            Serial.print(contador % 60);
        }
    }
    else
    {
        Serial.print(contador);
    }
}

/**
 * @brief Imprime por consola los valores que tiene el array.
 *
 * @param array El array que tiene los valores.
 */
void print_binary(int array[])
{
    for (i = LAST_LED; i >= FIRST_LED; i--)
    {
        Serial.print(array[i]);
    }
}

/**
 * @brief Imprime el meme de Spok de los simsons en ASCII.
 *
 */
void print_meme_simpsons()
{
    if (flagMeme == 0)
    {
        // Me hubiese gustado que sea mas grande y detallado pero arduino no me dejo :(.
        Serial.println();
        Serial.println();
        Serial.println("             BIEN, YA CUMPLI MI MISION AQUI      ");
        Serial.println();
        Serial.println("                        ,gggggggg,               ");
        Serial.println("                   ,g@@@@@@@@@@@@@@@p            ");
        Serial.println("                ,g@@@@@@@@@@@@@@@@@@@@,          ");
        Serial.println("              ,@@@@@@@@@@@@@@@@@@@@@@@@p         ");
        Serial.println("            ;@@@@@@@@@@@@@N@@$@N@@@@@@@@         ");
        Serial.println("           *F7==W&wW||g@@NW~||L$@@@@@@@@h        ");
        Serial.println("               ($A4/LLLl&&l|LlL$$@@@@@@@         ");
        Serial.println("               A8kgLLLLQm8W$L|TL|@|||T@P         ");
        Serial.println("              }LLI$LLLLLLLLLLLLLLL$LLL]          ");
        Serial.println("               Q2|LLL|||>|LLLLL9LL2LLLJ          ");
        Serial.println("                WLLLW|LLL|TG+L|LLLLLL|C          ");
        Serial.println("                W|eTLLLLLLLL|g|l|L|J7.           ");
        Serial.println("                 ]LLLLLLLLLg$|lLLLLL,            ");
        Serial.println("                ]|LLLL|leT|LLLLLLLLL$@g          ");
        Serial.println("                     ?**@LLLLLLLL|g@@@@@@;       ");
        Serial.println("                    ,am@$@@@@@@@@@@@@@@@@$@@g    ");
        Serial.println("                -*PPPPPPPPPPPPPPPPPPPPPPPPPPPPP* ");
        flagMeme = 1;
    }
}

/**
 * @brief Impime por consola "PAUSE" cuando habilitar esta en 0,
 * o "CONTINUE" si se vuelve a presionar el boton, pero solo si la secuencia
 * ya habia iniciado..
 *
 */
void print_pause_continue()
{
    if (habilitar == 0)
    {
        Serial.println();
        Serial.println();
        Serial.print("\t\t     ----> ");
        Serial.print("PAUSE");
        Serial.print(" <----");
        Serial.println();
    }
    else
    {
        if (flagStart == 1 || flagReset == 1)
        {
            Serial.println();
            Serial.print("\t\t    <---- ");
            Serial.print("CONTINUE");
            Serial.print("  ----> ");
            Serial.println("");
        }
    }
}

/**
 * @brief Imprime por consola "RESET".
 *
 */
void print_reset()
{
    Serial.println();
    Serial.print("\t\t     ----> ");
    Serial.print("RESET");
    Serial.print(" <----");
    Serial.println();
}

/**
 * @brief Muestra por consola el titulo re lindo, "START" al inicio,
 * y despues las horas, minutos y segundos en decimal
 * y los segundos en binario.
 *
 * @param contador El contador para imprimir en decimal.
 * @param array El array con los valores para imprimir en binario.
 */
void print_time(int contador, int array[])
{
    print_cronometro_start();
    Serial.println("");
    Serial.print("\t   |<--- Binario ");
    Serial.print("| TIEMPO |");
    Serial.print(" Decimal --->|");
    Serial.println("  ");
    Serial.print("\t      ");
    print_binary(array);
    Serial.print("            ");
    print_horas(contador);
    Serial.print(":");
    print_minutos(contador);
    Serial.print(":");
    print_segundos(contador);
    Serial.print("    ");
}

/**
 * @brief Sequencia que va a contar y mostrar el tiempo cada 1 segundo.
 *
 */
void sequence()
{
    unsigned long millis_now = millis();
    if (millis_now - millis_before >= TIME)
    {
        if (contador <= MAX_SECONDS)
        {
            binary(contador, arrayEnteros);
            leds(arrayEnteros);
            print_time(contador, arrayEnteros);
            contador++;
        }
        else
        {
            print_meme_simpsons();
        }

        millis_before = millis_now;
    }
}

/**
 * @brief Antirebote el boton start, que es el que inicia el la secuencia y/o la pasusa.
 *
 */
void press_button_start()
{
    int button_start_now = digitalRead(BUTTON_START);
    if (button_start_now == HIGH && button_start_before == LOW)
    {
        habilitar = !habilitar;
        print_pause_continue();
        flagStart = 1;
    }
    button_start_before = button_start_now;
}

/**
 * @brief Anti rebote del boton reset, que es el que reinicia el contador.
 *
 */
void press_button_reset()
{
    int button_reset = digitalRead(BUTTON_RESET);
    if (button_reset == HIGH && button_reset_before == LOW)
    {
        contador = ZERO_TO_HERO;
        print_reset();
        flagReset = 1;
    }

    button_reset_before = button_reset;
}

void loop()
{
    press_button_start();
    if (habilitar)
    {
        sequence();
    }
    press_button_reset();
    delay(80); //<------ Con un delay mas bajo el boton de inicio funcionaba raro.
}