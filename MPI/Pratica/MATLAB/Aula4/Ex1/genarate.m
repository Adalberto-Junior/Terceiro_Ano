function string = genarate(N, imin, imax,symb)
%if nargin < 4
%    symb = ['a':'z' 'A':'Z'] parte B 
%end

string = cell(N,1);
comprimento = randi([imin,imax],1,N);

for i = 1:N
    pos = randi([1 ,length(symb)], 1, comprimento(i));
    word = symb(pos);
    string{i} = word;
end

end
