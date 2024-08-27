# Projeto de Sensor de Combustível com Filtro Deslizante e Descarte de Outliers

Este projeto consiste em um código para leitura de um sensor de potenciômetro linear instalado no tanque de combustível, desenvolvido por Gustavo Garcia e Felipe Silva, em 26/08/2024.

## Descrição do Projeto

Devido à presença de ruídos nas leituras do sensor, decidimos implementar um tratamento de dados para suavizar as variações bruscas e anômalas no gráfico de porcentagem de combustível x tempo.
Após testar diferentes métodos, o filtro deslizante com descarte de outliers se mostrou o mais eficaz.

### Filtro Deslizante com Descarte de Outliers

O filtro deslizante funciona calculando a média das últimas 10 leituras do sensor. Qualquer leitura que ultrapasse os limites predefinidos (780 e 995) é descartada e substituída pela última leitura válida. 
Isso permite eliminar picos anômalos que poderiam distorcer o resultado, proporcionando uma leitura mais estável.

### Imagens do Projeto

- Foto do sensor:

<p align="center">
  <img src="sensor.jpg" alt="Sensor de Combustível" width="600" style="display: block; margin-left: auto; margin-right: auto;"/>
</p>

<br>

- Gráfico obtido no Serial Plot após a aplicação do filtro:

<p align="center">
  <img src="serialplot.png" alt="Foto do sensor funcionando no SerialPlot" width="600" style="display: block; margin-left: auto; margin-right: auto;"/>
</p>

## Código-Fonte - [potenc_sensor_tanque_com_filtro_deslizante.ino](https://github.com/gustavogpg/Panthera/blob/bbb175773a418e55178582d415d0220bc9fe3aba/arduino/sensores/sensor%20do%20tanque/potenc_sensor_tanque_com_filtro_deslizante.ino)

```cpp
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

