%plot mesh using OSPRAY
function ospsurf = ospraytsurf(F,V, fig)
    
    if nargin < 3
        f = gcf;
    else
        f = fig;
    end
   
    CameraPosition = [];
    CameraUpVector = [];
    CameraTarget = [];
    CameraFOV = [];
    
    %ospray handle automatically deleted on close
    if(strcmp(f.NextPlot,'add'))
        if(~isprop(f, 'ospray'))
            f.addprop('ospray');
            f.ospray = osprayplot_handle;
            
            tsurf(F,V);
    
            CameraPosition = campos;
            CameraUpVector = camup;
            CameraTarget = camtarget;
            CameraFOV = camva;
        
        end
        
        pos = f.Position;
        clf(f);
        hold on;
        imshow(zeros(pos(4), pos(3)));
        hold off;
        
        InSet = get(gca, 'TightInset');
        set(gca, 'Position', [InSet(1:2), 1-InSet(1)-InSet(3), 1-InSet(2)-InSet(4)]);
        
    else
        %create figure
        f = figure;
        
        tsurf(F,V);
    
        CameraPosition = campos;
        CameraUpVector = camup;
        CameraTarget = camtarget;
        CameraFOV = camva;
    
        %figure needs its own ospray handle
        f.addprop('ospray');
        f.ospray = osprayplot_handle;
        
        pos = f.Position;
        clf(f);
        hold on;
        imshow(zeros(pos(4), pos(3)));
        hold off;
        
        InSet = get(gca, 'TightInset');
        set(gca, 'Position', [InSet(1:2), 1-InSet(1)-InSet(3), 1-InSet(2)-InSet(4)]);
            
    end
   
    %add object
    ospsurf = f.ospray.addObject(F,V);
    
    if ospsurf.ObjectID == 0
        f.ospray.setCamera(CameraPosition, (CameraTarget-CameraPosition)./norm(CameraTarget-CameraPosition), CameraUpVector, 0.1, CameraFOV)
        f.SizeChangedFcn = @sizeChangedFcn;
        f.Color = [1,1,1];
    end
    
    drawnowOspray(f);

    function sizeChangedFcn(hObject, eventdata, handles)
        f.ospray.dirty = true;
        f.ospray.render([f.Position(4) f.Position(3)]);
        f.CurrentAxes.Children.CData = f.ospray.current_frame;
        drawnowOspray(f);
    end

       
end