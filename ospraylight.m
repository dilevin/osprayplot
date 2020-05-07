function h = ospraylight(type)
    
    h = [];
    f = gcf;
    
    if(~isprop(f, 'ospray'))
        error('ospray not initialized');
        return;
    end
    
    if ~any(strcmp(f.ospray.lights_pathtracer, type))
        error(['Light of type ' type 'not supported, no new light added']);
    else 
        h = f.ospray.addLight(type);
    end
                        
    
end