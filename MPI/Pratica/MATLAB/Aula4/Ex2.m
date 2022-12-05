%2. Considere a func¸ao Matlab ˜ string2hash()1 que implementa duas func¸oes de dispers ˜ ao diferentes. ˜
%Considere ainda 2 func¸oes Matlab fornecidas que s ˜ ao adaptac¸ ˜ oes para Matlab das func¸ ˜ oes de dispers ˜ ao˜
%hashstring()2 e DJB31MA()3.
%Utilizando separadamente cada uma destas quatro func¸oes de dispers ˜ ao, simule a inserc¸ ˜ ao das chaves ˜
%criadas no exerc´ıcio 1a) em 3 Chaining Hash Tables, uma de tamanho 5 ×105 , outra de tamanho 106
%e a terceira de tamanho 2 × 106 . Para cada uma das simulac¸oes (4 func¸ ˜ oes de dispers ˜ ao˜ × 3 tamanhos):

%%
%gerar chaves
N = 1e5;
imin = 6;
imax = 20;
symb = ['a':'z' 'A':'Z'];
word = genarate(N, imin,imax,symb);

%sizewor = size(word);
%%
%T = 1e3
fprintf("dados de T = 10^3\n");
T = 1e3; %tamande palavras
hascodes = zeros(1,T);
num_atribuicoes = zeros(1,T);

for i = 1:T
    tic 
    %word{i}
    hash= string2hash(word{i});
    hash = rem(hash,T)+1;
    hashcodes(i) = hash;
    num_atribuicoes(hash) = num_atribuicoes(hash)+1;
end
    toc;
n_colisoes = sum(num_atribuicoes > 1);
fprintf("Números de colisões: %d\n",n_colisoes);
fprintf("\n")
fprintf("Dados de hashstring:\n")
hascodeshastr = zeros(1,T);
num_atribuicoes = zeros(1,T);

for i = 1:T
    tic 
    %word{i}
    hash= hashstring(word{i},T);
    hash = rem(hash,T)+1;
    hascodeshastr(i) = hash;
    num_atribuicoes(hash) = num_atribuicoes(hash)+1;
end
    toc;
n_colisoes = sum(num_atribuicoes > 1);
fprintf("Números de colisões: %d\n",n_colisoes);
fprintf("\n")

%%
%T = 5 * 10^5
fprintf("dados de T = 5 * 10^5\n")
T1 = 5 * 1e5;
word = genarate(T1, imin,imax,symb);
%wordsiz = size(word);
hascodes1 = zeros(1,T1);
num_atribuicoes = zeros(1,T1);

for i = 1:T1
    tic 
    %word{i}
    hash = string2hash(word{i});
    hash = rem(hash,T1)+1;
    hashcodes1(i) = hash;
    num_atribuicoes(hash) = num_atribuicoes(hash)+1;
end
    toc
n_colisoes = sum(num_atribuicoes > 1);
fprintf("Números de colisões: %d\n",n_colisoes);
fprintf("\n")
%%
%T = 10^6 
fprintf("dados de T = 10^6\n");
T2 = 1e6;
word = genarate(T2, imin,imax,symb);
%wordsiz = size(word);
hascodes2 = zeros(1,T2);
num_atribuicoes = zeros(1,T2);

for i = 1:T2
    tic 
    %word{i}
    hash= string2hash(word{i});
    hash = rem(hash,T2)+1;
    hashcodes2(i) = hash;
    num_atribuicoes(hash) = num_atribuicoes(hash)+1;
end
    toc

n_colisoes = sum(num_atribuicoes > 1);
fprintf("Números de colisões: %d\n",n_colisoes);
fprintf("\n");


%%
%T = 2 * 10^6
fprintf("dados de T = 2 * 10^6\n");
T3 = 2 * 1e6;
word = genarate(T3, imin,imax,symb);
%wordsiz = size(word);
hascodes3 = zeros(1,T3);
num_atribuicoes = zeros(1,T3);

for i = 1:T3
    tic 
    %word{i}
    hash= string2hash(word{i});
    hash = rem(hash,T3)+1;
    hashcodes3(i) = hash;
    num_atribuicoes(hash) = num_atribuicoes(hash)+1;
end
    toc
n_colisoes = sum(num_atribuicoes > 1);
fprintf("Números de colisões: %d\n",n_colisoes);

