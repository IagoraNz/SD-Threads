# 🚀 SD-Threads  

Projeto acadêmico desenvolvido em **C++17**, utilizando **Threads, Middleware, Docker** e arquitetura **Mestre–Escravo** para processamento distribuído.  

## 🎯 Objetivo  
Implementar um **sistema distribuído** em C++ puro, onde:  
- Um **Cliente** envia arquivos `.txt` contendo letras e números.  
- Um **Mestre** recebe as requisições, dispara **threads paralelas** e distribui o processamento.  
- Dois **Escravos** processam os dados (um conta letras e outro conta números).  
- O **resultado consolidado** é devolvido ao Cliente em formato JSON.  

## 🧩 Arquitetura  

📌 **Cliente (Notebook 1)**  
- Envia arquivo `.txt`.  
- Interface **linha de comando** ou **gráfica (Qt/GTK+)**.  
- Faz requisições HTTP (REST) ao **Mestre**.  
- Apenas envia e recebe dados (não processa).  

📌 **Mestre (Container 1)**  
- Recebe requisições do Cliente.  
- Dispara **2 threads paralelas**, cada uma se comunica com um Escravo.  
- Verifica disponibilidade dos Escravos (`/health`).  
- Consolida as respostas e retorna JSON ao Cliente.  

📌 **Escravos (Containers 2 e 3)**  
- **Escravo 1** → endpoint `/letras` → conta letras.  
- **Escravo 2** → endpoint `/numeros` → conta números.  

## 🛠️ Tecnologias  

- **Linguagem:** C++17 ou superior  
- **Concorrência:** `std::thread`, `std::async`  
- **Comunicação REST:** [cpp-httplib](https://github.com/yhirose/cpp-httplib) ou Boost.Beast  
- **Contêineres:** Docker + Docker Compose  
- **Arquitetura:** Mestre–Escravo com endpoints REST  

## 🗂️ Estrutura do Repositório  

```
SD-Threads/  
├── 📁 cliente/  
│   ├── 🖥️ cliente.cpp  
│   └── 📄 CMakeLists.txt  
│  
├── 📁 mestre/  
│   ├── 🖥️ mestre.cpp  
│   └── 📄 CMakeLists.txt  
│  
├── 📁 escravo1/  
│   ├── 🖥️ escravo1.cpp  
│   └── 📄 CMakeLists.txt  
│  
├── 📁 escravo2/  
│   ├── 🖥️ escravo2.cpp  
│   └── 📄 CMakeLists.txt  
│  
├── 🐳 docker-compose.yml  
├── 🐳 Dockerfile  
└── 📘 README.md  
```
