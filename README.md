# graham-scan

A descrição de compilação está no arquivo Compile.txt

Um conjunto de vértices é gerado pelo usuário dinamicamente pela execução do arquivo objeto executável gerado pelo arquivo "openGL.cpp" e inserido no arquivo "points.txt". Esse conjunto não ordenado de vértices é então lido pelo arquivo objeto executável gerado pelo arquivo graham_scan.c que então cria um fecho convexo, usando uma implementação do algoritmo "Graham scan", então imprimindo esses vértices, ordenadamente no arquivo "loop.txt". Esse novo conjunto de vértices então é lido novamente pelo executável gerado a partir de "openGL.cpp" e as arestas que os ligam são mostradas no display, em loop.
