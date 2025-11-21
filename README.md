# Simulador de Caravanas

## Descrição do Projeto
Este projeto tem como objetivo criar um simulador de caravanas desenvolvido em C++, realizado no âmbito da unidade curricular de Programação Orientada a Objetos. O sistema simula o movimento e gestão de diferentes tipos de caravanas num mapa, incluindo comércio, combate, gestão de recursos e eventos aleatórios.

## Equipa de Desenvolvimento
- **Frederico Quelhas** . [@FredericoQuelhas20](https://github.com/FredericoQuelhas20)
- **Afonso Silva** . [@Afonsod706](https://github.com/Afonsod706)

## Tecnologias Utilizadas
- **C++** (Standard moderno)
- **CMake** para gestão de build
- **CLion** como IDE (recomendado)
- **GitHub** para controle de versão

## Funcionalidades
- Simulação de diferentes tipos de caravanas (Comercial, Militar, Secreta, Bárbara)
- Sistema de mapa configurável com cidades e terrenos variados
- Gestão de recursos (água, tripulação, carga, moedas)
- Sistema de comércio entre caravanas e cidades
- Combate entre caravanas e bárbaros
- Itens especiais com efeitos diversos (Caixa de Pandora, Baú do Tesouro, Jaula, Mina, Surpresa)
- Modo autónomo para caravanas com comportamentos inteligentes
- Sistema de eventos (tempestades, aparecimento de bárbaros)
- Buffer de visualização para renderização do mapa
- Sistema de turnos e progressão temporal
- Gravação e carregamento de estados do jogo

## Instalação

### Requisitos
- Compilador C++ com suporte a C++11 ou superior (g++, clang++, MSVC)
- CMake versão 3.10 ou superior

### Passos de Instalação

1. Clone o repositório do GitHub:
```bash
git clone https://github.com/FredericoQuelhas20/Simulador-de-Caravanas.git
cd Simulador-de-Caravanas
```

2. Certifique-se de que o compilador C++ está instalado:
```bash
g++ --version
# ou
clang++ --version
```

3. Compile o projeto com CMake:
```bash
mkdir build
cd build
cmake ..
make
```

4. Execute a aplicação:
```bash
./Simulador-de-Caravanas
```

## Estrutura do Projeto
```
/src                  - Ficheiros de código-fonte (.cpp e .h)
  ├── main.cpp        - Ponto de entrada da aplicação
  ├── Simulator.cpp/h - Classe principal do simulador
  ├── Map.cpp/h       - Gestão do mapa e entidades
  ├── Caravan.cpp/h   - Classes das caravanas (Comercial, Militar, Secreta, Bárbara)
  ├── City.cpp/h      - Gestão de cidades e comércio
  ├── Item.cpp/h      - Itens especiais e seus efeitos
  └── Buffer.cpp/h    - Sistema de buffer para visualização
/reports              - Relatórios e documentação do projeto
/cmake-build-debug    - Ficheiros de build (gerados)
/.idea                - Configurações do IDE
CMakeLists.txt        - Configuração do CMake
README.md             - Documentação do projeto
```

## Como Utilizar

### Comandos Básicos
O simulador aceita vários comandos para interagir com o jogo:

**Gestão de Caravanas:**
- Criar caravanas em cidades
- Mover caravanas pelo mapa (direções: N, S, E, W, NE, NW, SE, SW)
- Ativar/desativar modo autónomo

**Comércio:**
- Comprar mercadorias nas cidades
- Vender mercadorias nas cidades
- Contratar tripulação
- Comprar novas caravanas

**Informação:**
- Listar caravanas existentes
- Consultar informações de cidades
- Verificar preços de mercadorias
- Visualizar itens no mapa

**Simulação:**
- Avançar turnos manualmente
- Executar comandos de ficheiro
- Guardar/carregar estado do jogo

### Tipos de Caravanas
1. **Comercial**: Especializada em transporte de mercadorias (20 tripulantes, 40 toneladas de carga)
2. **Militar**: Focada em combate (40 tripulantes, 5 toneladas de carga)
3. **Secreta**: Caravana especial com características únicas (50 tripulantes, 10 toneladas)
4. **Bárbara**: Caravanas hostis controladas pelo sistema (2 tripulantes, comportamento agressivo)

### Itens Especiais
- **Caixa de Pandora**: Efeito aleatório (positivo ou negativo)
- **Baú do Tesouro**: Adiciona moedas à caravana
- **Jaula**: Aumenta a tripulação
- **Mina**: Causa danos à caravana
- **Surpresa**: Efeito misterioso

## Compilação e Execução

### Com CMake (Recomendado)
```bash
# Criar diretório de build
mkdir build && cd build

# Configurar o projeto
cmake ..

# Compilar
cmake --build .

# Executar
./Simulador-de-Caravanas
```

### Compilação Manual
```bash
g++ -std=c++11 -o simulador main.cpp Simulator.cpp Map.cpp Caravan.cpp City.cpp Item.cpp Buffer.cpp
./simulador
```

## Exemplo de Utilização
```cpp
// Inicializar simulador com mapa 10x20
Simulator simulator(10, 20);

// Iniciar o simulador
simulator.start();

// O simulador entra em modo interativo
// Aceita comandos via stdin ou ficheiros de configuração
```

## Testes
Os testes e validações do projeto estão documentados na pasta `/reports`. Para verificar o funcionamento:

1. Execute o simulador
2. Teste os comandos básicos
3. Verifique o comportamento autónomo das caravanas
4. Valide o sistema de combate e comércio

## Funcionalidades Avançadas
- **Comportamento Autónomo**: Caravanas podem operar automaticamente
- **Sistema de Combate**: Resolução automática baseada em tripulação
- **Economia Dinâmica**: Preços variam entre cidades
- **Eventos Aleatórios**: Tempestades e aparecimento de bárbaros
- **Persistência**: Capacidade de guardar e carregar jogos

## Licença
Este projeto está licenciado sob a Licença MIT - veja o ficheiro LICENSE para mais detalhes.

## Contacto
Para questões:
- **Frederico Quelhas** - [@FredericoQuelhas20](https://github.com/FredericoQuelhas20)

---

**Nota:** Projeto académico desenvolvido para a unidade curricular de Programação Orientada a Objetos.
