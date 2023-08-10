# Fifa2023-1
Trabalho final da disciplina: "Classificação e Pesquisa de Dados" <br>

## Tipos de pesquisa:

<ol>
  <li> Pesquisa sobre os nomes dos jogadores <br>
  - Uso de uma <strong>Trie</strong> que armazena, nos nodos onde terminam o nome de um jogador, o id do jogador <br>
  - Uso de uma <strong>Tabela Hash</strong> em que cada elemento contém: o id, o nome, o número de reviews e a soma dos reviews de um jogador
  </li>
  <li> Pesquisa sobre jogadores revisados por usuários <br>
  - Uso de uma <strong>Tabela Hash</strong> em que cada elemento é um <strong>Heap</strong> de no máx. 20 elementos que contém o id do jogador e a nota que esse usuário o deu. <br>
  - Esse heap guarda o jogador de menor nota no topo, para que ele seja retirado caso seja feita a inserção de um jogador de nota maior.
  </li>
  <li> Pesquisas sobre os melhores jogadores de uma determinada posição <br>
  - Uso de um <strong>Vector</strong> que armazena os jogadores com mais de 1000 reviews. Eles só são inseridos depois que a Tabela Hash dos jogadores é criada. <br>
  - Uma vez inseridos, eles são ordenados usando o <strong>Heap Sort</strong>
  </li>
  <li> Pesquisas sobre as 'tags' dos jogadores <br>
  - Uso de uma <strong>Tabela Hash</strong> com mapeamento string -> int em que cada elemento é uma <strong>Tabela Hash</strong> dos ids dos jogadores. <br>
  - É escolhida a tabela de menor tamanho como resposta inicial. É verificado, então, se cada elemento dessa tabela está nas outras tags da busca. Se não estiver, ele é retirado da resposta.
  </li>
</ol>

## Structs utilizadas nas estruturas de dados

<li>Struct dos jogadores (nodo da Hash de jogadores): <br></li>

```cpp
  typedef struct player_struct {
    int id;
    string name;
    unsigned long long n_reviews, sum_reviews;
  } Player;
```

<li>Struct dos reviews (nodo do Heap): <br></li>

```cpp
  typedef struct review_struct {
    int id;
    int review;
  } Review;
```

