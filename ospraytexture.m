function h = ospraytexture(texture)
    
    h = [];
    f = gcf;
    
    if(~isprop(f, 'ospray'))
        error('ospray not initialized');
        return;
    end
    
    h = f.ospray.addTexture(texture);
                      
end