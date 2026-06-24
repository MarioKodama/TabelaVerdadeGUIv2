# 🧠 TabelaVerdadeGUI

Projeto em C++ com interface gráfica utilizando **Dear ImGui**, desenvolvido para gerar e simplificar tabelas verdade a partir de expressões lógicas.

---

## 📌 Sobre o projeto

O **TabelaVerdadeGUI** permite ao usuário inserir expressões lógicas e gerar automaticamente sua tabela verdade, além de fornecer uma forma simplificada da expressão.

A aplicação foi desenvolvida em C++ com foco em performance e estrutura de código direta, utilizando uma arquitetura baseada em funções inline para toda a lógica principal.

---

## ⚙️ Funcionalidades

- Geração de tabela verdade a partir de expressões lógicas
- Simplificação da expressão lógica
- Interface gráfica moderna utilizando Dear ImGui

---

## 🧮 Parsing de Expressões

Para interpretar e avaliar as expressões lógicas, foi utilizado o algoritmo **Shunting Yard**, responsável por converter a expressão infixa em uma forma pós-fixa (RPN), facilitando a avaliação de cada caso da tabela verdade.

### 🔄 Demonstração do algoritmo Shunting Yard

![Shunting Yard Demo](https://i.imgur.com/Sl8tCyI.gif)

✏️ Simplificação lógica

Para a simplificação das expressões, foi utilizada uma versão simplificada do algoritmo de Quine-McCluskey, adaptada para o escopo do projeto, permitindo reduzir expressões booleanas de forma eficiente.

### 🔣 Operadores suportados

O sistema suporta os seguintes operadores lógicos:

& → AND (conjunção)
| → OR (disjunção)
-> → Implicação lógica
== → Bicondicional
~ → NOT (negação)

Exemplo de expressão:

A & (B | ~C) -> D
🧱 Estrutura do projeto

Toda a lógica principal da aplicação está concentrada no arquivo:

incl.h

A implementação foi feita de forma inline, visando simplicidade e desempenho.
