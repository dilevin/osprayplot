%deal with colors
function Cout = ospray_colors(Cin, numVerts) 
    
    Cout = Cin;
    
    if size(Cin,1) ~= numVerts
        Cout = repmat(Cin, numVerts,1);
    end
    
    
end