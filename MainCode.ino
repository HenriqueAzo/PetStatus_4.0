#include <Keypad.h>
#include <Wire.h> // Library for I2C communication
#include <LiquidCrystal_I2C.h> // Library for LCD
#include <EEPROM.h>


const byte ROWS = 4;
const byte COLS = 4;
 char keys[ROWS][COLS] = {// matriz que inclui as teclas usadas
  {'1','2','3', 'A'},          //
  {'4','5','6', 'B'},          //
  {'7','8','9', 'C'},          //
  {'*','0','#', 'D'}           //
};                        //

byte rowPins[ROWS] = {2, 3, 4, 5}; //pinos usados para ler as linhas do teclado
byte colPins[COLS] = {6, 7, 8, 9}; //pinos usados para ler as colunas do teclado


Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );//determina os parâmetros do teclado

LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2);


//FIM DO SETUP IMUTAVEL

String input = "";
unsigned long int inputint = 0;
unsigned long int senha = 0;
unsigned int user = 0;
String asteriscos = "";
int state = 0;

//função para abrir a porta
void abrirporta(){
  digitalWrite(10, HIGH);
  delay(500);
  digitalWrite(10, LOW);
  Serial.println();
  Serial.println("a porta foi aberta!");
  Serial.println();
}

void reconhecimento_eeprom(){//função para verificar se a senha inserida é correta    
  //int para armazenamento de leitura da memória e comparação das senhas
  long int senhateste = 0;
  for(int i = user ;i<(user+6) ;i++){ //Busca os 6 caracteres associados ao usuario
    senhateste +=EEPROM.read(i);
    if(i-user<5){
      senhateste = senhateste*10;
    }
  }
  Serial.println();
  Serial.print("Usuário inserido: ");
  Serial.print(user);
  Serial.println();
  Serial.print("Senha inserida: ");
  Serial.print(senha);
  Serial.println();
  Serial.println("Senha registrada: ");
  Serial.print(senhateste);
  Serial.println();

  if(senha == senhateste){ // Se a senha for igual à senha associado ao usuario
  lcd.clear();
  lcd.setCursor(4, 0);
  lcd.print("Bem Vindo,");
  lcd.setCursor(5, 1);
  lcd.print("Usuario ");
  lcd.print(user);
  abrirporta();
  }else{
  Serial.println("Senha não reconhecida.");
  }
  state = 0;
}
 
void telaInicial(){
   lcd.init();
   lcd.clear();
   lcd.backlight();
   lcd.setCursor(0, 0); // Set the cursor on the third column and first row.
   lcd.print("PET Status");
   lcd.setCursor(0, 1);
   lcd.print("Iniciando...");
   digitalWrite(10, LOW);
}

void telaUser(){
  lcd.clear();
   lcd.backlight();
   lcd.setCursor(0, 0); // Set the cursor on the third column and first row.
   lcd.print("   PET Status");
   lcd.setCursor(0, 1);
   lcd.print("Usuario: ");
}

void telaSenha(){
  lcd.clear();
   lcd.backlight();
   lcd.setCursor(0, 0); // Set the cursor on the third column and first row.
   lcd.print("   PET Status");
   lcd.setCursor(0, 1);
   lcd.print("Senha: ");
}

void setup() {
  Serial.begin(9600);
  pinMode(10, OUTPUT);//pino de acionamento da porta

  telaInicial();
  Serial.println("iniciando o sistema");
   
}

void loop() {
  //início da FSM
  switch (state){
    
    //apresenta a tela inicial
    case 0:
    Serial.println("tela inicial foi enviada");
    telaUser();
    input = "";
    senha = 0;
    user = 0;
    state++;
    Serial.println("state=1, esperando input de usuario");
    break;

    //aguarda usuário ter 2 dígitos e confere se usuário inserido foi de 0 a 30, do contrário, retorna à tela inicial
    case 1:
    if(input.length()==2){
      inputint = input.toInt();
      if (inputint>=0&& inputint<31){
        user = inputint;
        inputint = 0;
        Serial.print("usuario registrado: ");
        Serial.println(user);
        state++;
        Serial.println("state=2, reconhecendo o usuario");
      }
    }

    break;

    //apresenta tela de senha e limpa o input para receber novos dados
    case 2:
    telaSenha();
    input = "";
    state++;
    Serial.println("state=3, tela de senha enviada ao LCD, esperando senha de 6 digitos");
    break;
    
    //espera o input de uma senha de 6 dígitos e armazena o inserido no int senha. no final, confere a senha e abre ou não a porta.
    case 3:
    if (input.length()==6){
      inputint = input.toInt();
      Serial.print("input: ");
      Serial.println(input);
      Serial.print("inputint: ");
      Serial.println(inputint);
      senha = inputint;
      inputint = 0;
      Serial.print("senha registrada no case 3: ");
      Serial.println(senha);

      reconhecimento_eeprom();
    }
    break;

    default:
    state = 0;
    break;
  }

  char key = keypad.getKey();
  if (key != NO_KEY){//segura o código enquanto o programa não recebe input    
    char lastkey = key;
    lcd.print(lastkey);
    input.concat(key);//adiciona a tecla pressionada no final da String que é a senha
    
    if (input.indexOf("#") != -1){//limpa a senha caso seja inserido um #
        input = "";
        asteriscos = "";
      }else{
        asteriscos.concat('*');
      }
    
    Serial.print("inserido: ");
    Serial.println(input);
    Serial.print("apertado: ");
    Serial.println(asteriscos); //coloca a tecla apertada no monitor serial
    Serial.println();
}
}//FIM DO VOID LOOP
