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
  delay(200);
  digitalWrite(10, LOW);
  Serial.println();
  Serial.println("a porta foi aberta!");
  Serial.println();
}

void registro_eeprom(){
  int pos=0;//valor para a posição da string de 0 a 5 na senha

  for(int i = user*6 ;i<((user*6)+6) ;i++){ //Busca os 6 números associados ao usuario na memória EEPROM do arduino. para usuario 0, busca dos bytes 0 a 5 e os concatena formando um número, por exemplo. para usuário 1, seriam os bytes de 6 a 11, e assim por diante.
    int digit = input.charAt(pos) - '0';
    pos++;
    EEPROM.write(i, digit);
    Serial.println();
    Serial.print("escrito o dígito ");
    Serial.print(digit);
    Serial.print(" no endereço ");
    Serial.print(i);
    Serial.println(".");
  }
  Serial.println();
  Serial.print("novo usuário inserido: ");
  Serial.print(user);
  Serial.println();
  Serial.print("nova senha inserida: ");
  Serial.print(input);
  Serial.println();
  Serial.println("Senha registrada: ");
  Serial.print(input);
  Serial.println();  
}

void reconhecimento_eeprom(){//função para verificar se a senha inserida é correta    
  //int para armazenamento de leitura da memória e comparação das senhas
  long int senhateste = 0;
  for(int i = user*6 ;i<((user*6)+6) ;i++){ //Busca os 6 números associados ao usuario na memória EEPROM do arduino. para usuario 0, busca dos bytes 0 a 5 e os concatena formando um número, por exemplo. para usuário 1, seriam os bytes de 6 a 11, e assim por diante.
    senhateste +=EEPROM.read(i);
    if(i-(user*6)<5){
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

  if(senha == senhateste && user == 0){//caso de usuario admin, para registrar novas senhas
    telaNovoUser();

    Serial.println("iniciando modo de administrador");
    state=4;
    Serial.println("aguardando input de novo usuario");

    input = "";
    inputint = 0;
    senha = 0;
    user = 0;
  }else if(senha == senhateste && user == 1){
    lcd.clear();
    lcd.setCursor(4, 0);
    lcd.print("Bem Vindo,");
    lcd.setCursor(2, 1);
    lcd.print("Tutor");
    abrirporta();
    state = 0;
  }else if(senha == senhateste){ // Se a senha for igual à senha associado ao usuario
    lcd.clear();
    lcd.setCursor(4, 0);
    lcd.print("Bem Vindo,");
    lcd.setCursor(2, 1);
    lcd.print("Petiano(");
    lcd.print(user);
    lcd.print(")");
    abrirporta();
    state = 0;
  }else{
    Serial.println("Senha não reconhecida.");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("    Senha Nao");
    lcd.setCursor(2, 1);
    lcd.print("Reconhecida");
    delay(500);
    state = 0;
  }
  
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
   lcd.print("PET Status");
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

void telaNovoUser(){
  lcd.clear();
  lcd.backlight();
  lcd.setCursor(0, 0); // Set the cursor on the third column and first row.
  lcd.print("Novo Usuario:");
  lcd.setCursor(0, 1);
}

void telaNovaSenha(){
  lcd.clear();
  lcd.backlight();
  lcd.setCursor(0, 0); // Set the cursor on the third column and first row.
  lcd.print("Nova senha:");
  lcd.setCursor(0, 1);
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
      inputint = 0;
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
          Serial.println("state=2, registrando usuario inserido e limpando input");
        }else{
          state=0;
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

    case 4:
      if(input.length()==2){
        inputint = input.toInt();
        if (inputint>=0&& inputint<31){
          user = inputint;
          inputint = 0;
          Serial.print("novo usuario inserido: ");
          Serial.println(user);
          state=5;
          Serial.println("state=5, usuario registrado e limpando o input");
        }
      }
    break;

    case 5:
      telaNovaSenha();
      input = "";
      state=6;
      Serial.println("state=6, tela de senha enviada ao LCD, esperando nova senha de 6 digitos");
    break;

    case 6:
      if (input.length()==6){
        inputint = input.toInt();
        Serial.print("input: ");
        Serial.println(input);
        Serial.print("inputint: ");
        Serial.println(inputint);
        senha = inputint;
        inputint = 0;
        Serial.print("senha registrada no case 6: ");
        Serial.println(senha);

        registro_eeprom();
        state=0;
        Serial.println("retornando ao stage 0...");
        Serial.println();
        Serial.println();
      }      
    break;

    default:
    state = 0;
    break;
  }

  char key = keypad.getKey();
  if (key != NO_KEY){//segura o código enquanto o programa não recebe input    
    char lastkey = key;
    lcd.print("#");
    input.concat(key);//adiciona a tecla pressionada no final da String que é a senha
    
    if (input.indexOf("#") != -1){//limpa a senha caso seja inserido um #
        input = "";
        asteriscos = "";
        switch (state){
          case 1:
            telaUser();
          break;

          case 3:
            telaSenha();
          break;

          case 4:
            telaNovoUser();
          break;

          case 6:
            telaNovaSenha();
          break;
        
        default:
        state=0;
        break;
        }

    }else{
      asteriscos.concat('*');
    }

    if (input.indexOf("*") != -1){//limpa a senha caso seja inserido um #
        input = "";
        asteriscos = "";
        state=0;
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
