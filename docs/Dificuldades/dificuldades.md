# ⚠️ Dificuldades do Projeto

Durante o desenvolvimento do **Interpretador**, a equipe enfrentou alguns desafios importantes que impactaram diretamente nas decisões de projeto e na organização das entregas. As principais dificuldades foram:

---

## 🔎 Compilador x Interpretador
Um dos primeiros debates do grupo foi decidir se o sistema seria implementado como **compilador** ou como **interpretador**.  
- O compilador permitiria otimizações mais avançadas e geração de código eficiente, mas demandaria mais esforço e conhecimento em estágios como análise semântica, geração de código intermediário e final.  
- O interpretador, por outro lado, se mostrou mais viável para os prazos da disciplina, já que possibilita a execução direta da **AST (Abstract Syntax Tree)**, facilitando a validação das construções da linguagem.  
➡️ Após discussões, a equipe optou por seguir com o **interpretador**.

---

## 💻 Escolha da Linguagem de Implementação
Outro ponto de dificuldade foi escolher a linguagem para implementar o projeto.  
- Algumas opções levantadas: **Python e C**.  
- O **Python** oferecia facilidade de prototipagem, mas menor integração com Flex/Bison.  
- O **C**, apesar de exigir maior atenção à manipulação de memória e estruturas, mostrou-se o mais adequado por ser **compatível com Flex/Bison** e alinhar com os objetivos da disciplina.  
➡️ A decisão final foi implementar em **C**.

---

## 🛠️ Ferramentas e Integração
- Configurar corretamente o ambiente com **Flex e Bison** gerou dificuldades iniciais, especialmente para quem nunca havia trabalhado com essas ferramentas.  
- A equipe precisou alinhar versões, corrigir problemas de instalação e entender a estrutura mínima de arquivos `.l` e `.y`.  

---

## 👥 Organização em Equipe
- O trabalho em grupo também trouxe o desafio de dividir responsabilidades de forma equilibrada.  
- Nem todos tinham a mesma experiência com Git/GitHub, o que gerou dificuldades no início com versionamento e integração de código.  

---

## 📌 Conclusão
Apesar das dificuldades, esses desafios foram fundamentais para o aprendizado da equipe, que conseguiu avançar nas tomadas de decisão, melhorar a colaboração e desenvolver uma solução funcional dentro do prazo da disciplina.
