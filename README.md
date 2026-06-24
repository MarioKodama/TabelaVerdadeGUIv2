# 🧠 TabelaVerdadeGUI

Projeto em C++ com interface gráfica utilizando **Dear ImGui**, desenvolvido para gerar tabelas verdade a partir de expressões lógicas.

---

## 📌 Sobre o projeto

O **TabelaVerdadeGUI** permite ao usuário inserir expressões lógicas e gerar automaticamente sua tabela verdade, além de fornecer uma forma simplificada da expressão.

---

## ⚙️ Funcionalidades

- Geração de tabela verdade a partir de expressões lógicas
- Simplificação da expressão lógica
- Interface gráfica utilizando Dear ImGui

---

## 🧮 Parsing de Expressões

Para interpretar e avaliar as expressões lógicas, foi utilizado o algoritmo **Shunting Yard**, responsável por converter a expressão infixa em uma forma pós-fixa (RPN), facilitando a avaliação de cada caso da tabela verdade.

### 🔄 Demonstração do algoritmo Shunting Yard

![Shunting Yard Demo](https://i.imgur.com/Sl8tCyI.gif)

### ✏️ Simplificação lógica

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
### 🧱 Implementação dos algoritmos

Toda a lógica da tabela verdade está concentrada no arquivo:

incl.h


