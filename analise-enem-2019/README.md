# 📊 Análise e Modelagem Preditiva — ENEM 2019

Projeto de **Ciência de Dados** (UFAM, 2025.2) sobre a base de microdados do **ENEM 2019**, desenvolvido em dupla etapa: exploração e limpeza dos dados (TP1) seguida de um pipeline completo de aprendizado de máquina para prever o desempenho na redação (TP2). Trabalho em grupo com Cristiano Cardoso e Matheus Santarém.

![Python](https://img.shields.io/badge/Python-3776AB?style=for-the-badge&logo=python&logoColor=white)
![Pandas](https://img.shields.io/badge/Pandas-150458?style=for-the-badge&logo=pandas&logoColor=white)
![scikit-learn](https://img.shields.io/badge/scikit--learn-F7931E?style=for-the-badge&logo=scikit-learn&logoColor=white)
![Jupyter](https://img.shields.io/badge/Jupyter-F37626?style=for-the-badge&logo=jupyter&logoColor=white)

## 📓 TP1 — Exploração de Dados ([`tp1_exploracao_de_dados.ipynb`](tp1_exploracao_de_dados.ipynb))

Análise exploratória dos microdados do ENEM 2019 focada nos participantes de municípios específicos (Recife, entre outros):

- Diagnóstico e tratamento de dados ausentes (valores nulos em notas, códigos de prova e dados de nascimento).
- Substituição criteriosa de valores faltantes (ex.: notas ausentes por falta à prova) e criação de faixas etárias.
- Análises comparativas de desempenho por variáveis socioeconômicas: língua estrangeira escolhida, ocupação dos pais/responsáveis, entre outras — comparando sempre o município de interesse com o restante do país.
- Engenharia de atributos e seleção de variáveis por relevância estatística: correlação, ANOVA, qui-quadrado, informação mútua e importância de variáveis via Random Forest.

## 🤖 TP2 — Modelagem Preditiva ([`tp2_modelagem_preditiva.ipynb`](tp2_modelagem_preditiva.ipynb))

A partir da base tratada no TP1, o objetivo passa a ser **prever se um candidato obterá nota de redação acima da média** de sua cidade:

- Pipeline de pré-processamento justo entre os modelos: codificação one-hot para variáveis categóricas e padronização (z-score) para variáveis numéricas.
- Comparação de múltiplos algoritmos de classificação, cada um com otimização de hiperparâmetros via validação cruzada estratificada (`StratifiedKFold`).
- Teste estatístico de significância entre os modelos (teste de Wilcoxon) para embasar a escolha do melhor classificador.
- Interpretabilidade do modelo final com **SHAP**, explicando quais variáveis mais influenciam a previsão.

## 🛠️ Tecnologias e bibliotecas

`pandas` · `numpy` · `matplotlib` · `scikit-learn` (StandardScaler, RandomForestClassifier, seleção de atributos, model_selection) · `scipy.stats` · `shap`

## ▶️ Como executar

Os notebooks foram desenvolvidos no Google Colab, com os dados (`enem2019.parquet`) e os módulos de dados em um Google Drive montado no início do notebook. Para rodar localmente:

1. Baixe os microdados do ENEM 2019 (INEP) e ajuste os caminhos `DATA_PQT` / `base_dir` no início de cada notebook.
2. Instale as dependências: `pip install pandas numpy matplotlib scikit-learn scipy shap`.
3. Abra os notebooks com Jupyter (`jupyter notebook`) e execute as células em ordem.

## 👥 Autores

Davi Carlos, Cristiano Cardoso e Matheus Santarém — Engenharia de Software, UFAM.
