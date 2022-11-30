%2. Considere a func¸ao Matlab ˜ string2hash()1 que implementa duas func¸oes de dispers ˜ ao diferentes. ˜
%Considere ainda 2 func¸oes Matlab fornecidas que s ˜ ao adaptac¸ ˜ oes para Matlab das func¸ ˜ oes de dispers ˜ ao˜
%hashstring()2 e DJB31MA()3.
%Utilizando separadamente cada uma destas quatro func¸oes de dispers ˜ ao, simule a inserc¸ ˜ ao das chaves ˜
%criadas no exerc´ıcio 1a) em 3 Chaining Hash Tables, uma de tamanho 5 ×105 , outra de tamanho 106
%e a terceira de tamanho 2 × 106 . Para cada uma das simulac¸oes (4 func¸ ˜ oes de dispers ˜ ao˜ × 3 tamanhos):

N = 1e5;
imin = 6;
imax = 20;
symb = ['a':'z' 'A':'Z'];
word = genarate(N, imin,imax,symb);
T = 1e3 %tamande palavras
hascodes = zeros(size(word));
num_atribuicoes = zeros(1,T);

for i = 1:N
    tic 
    %word{i}
    hash= string2hash(word{i});
    hash = rem(hash,T)+1;
    hashcodes(i) = hash;
    num_atribuicoes(hash) = num_atribuicoes(hash)+1;
end
time = toc

%%
n_colisoes = sum(num_atribuicoes > 1);
fprintf("Números de colisões: %d\n",n_colisoes);



