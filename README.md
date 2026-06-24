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
- Suporte a múltiplos operadores lógicos
- Processamento eficiente de expressões complexas

---

## 🧮 Parsing de Expressões

Para interpretar e avaliar as expressões lógicas, foi utilizado o algoritmo **Shunting Yard**, responsável por converter a expressão infixa em uma forma pós-fixa (RPN), facilitando a avaliação de cada caso da tabela verdade.

### 🔄 Demonstração do algoritmo Shunting Yard

```md
![Shunting Yard Demo](shunting_yard.gif)
