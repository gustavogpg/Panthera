//------------------------------------------------------------
/* EESC - USP Guepardo / Gustavo Garcia + Felipe Silva
   Código para leitura do potenciômetro linear do sensor do tanque de combustível usando um tratamento de dados
   com filtro deslizante com descarte de outliers para suavizar a curva do gráfico (porcentagem de combustível x tempo)
   26/08/2024
*/ 

// Definições de constantes
#define ADC 0 // Porta analógica A0 onde o sensor está conectado
#define NUM_LEITURAS 10 // Número de leituras usadas na média móvel
#define LIMITE_SUPERIOR 995 // Valor superior para o descarte de outliers
#define LIMITE_INFERIOR 780 // Valor inferior para o descarte de outliers

// Variáveis globais
int leituras[NUM_LEITURAS]; // Array para armazenar as leituras do sensor
int soma_leituras = 0; // Soma das leituras, utilizada para calcular a média
int indice_leitura = 0; // Índice da leitura atual no array

void setup() {
  Serial.begin(9600); // Inicializa a comunicação serial a 9600 bps para monitoramento

  // Inicializa o array de leituras com zeros
  for (int i = 0; i < NUM_LEITURAS; i++) {
    leituras[i] = 0;
  }
}

void loop() {
  // Leitura da tensão analógica no pino A0:
  int leitura = analogRead(ADC); // Realiza a leitura do valor analógico do sensor

  // Descartar leitura se estiver fora dos limites definidos (outliers)
  if (leitura > LIMITE_SUPERIOR || leitura < LIMITE_INFERIOR) {
    leitura = leituras[indice_leitura]; // Se leitura for um outlier, substitui pela última leitura válida
  }

  // Atualiza a soma das leituras: subtrai a leitura mais antiga e adiciona a nova leitura
  soma_leituras -= leituras[indice_leitura];
  leituras[indice_leitura] = leitura; // Armazena a nova leitura no array
  soma_leituras += leitura;

  // Avança o índice circular para a próxima posição do array de leituras
  indice_leitura = (indice_leitura + 1) % NUM_LEITURAS;

  // Calcula a média das leituras armazenadas
  int media_leituras = soma_leituras / NUM_LEITURAS;

  // Mapeia a média calculada para a faixa de 0 a 100% de combustível no tanque
  int valor_tanque = map(media_leituras, 783, 993, 0, 100);

  // Garante que o valor da porcentagem esteja dentro da faixa válida (0% a 100%)
  int porcentagem_tanque = constrain(valor_tanque, 0, 100);

  // Envia a porcentagem de combustível calculada para o monitor serial
  Serial.println(porcentagem_tanque);

  // Aguarda 10 milissegundos antes de realizar a próxima leitura
  delay(10);
}
