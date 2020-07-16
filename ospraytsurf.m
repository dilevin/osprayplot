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
    
    holdon = false;
    
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
        
        holdon=true;
        
        pos = f.Position;
        clf(f);
        hold on;
        imshow(zeros(round(pos(4)), round(pos(3))));
        hold on;
        

        %hold off;
        
        InSet = get(gca, 'TightInset');
        set(gca, 'Position', [InSet(1:2), 1-InSet(1)-InSet(3), 1-InSet(2)-InSet(4)]);
        hold on;
        
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
        
        f.Position = pos;
        InSet = get(gca, 'TightInset');
        set(gca, 'Position', [InSet(1:2), 1-InSet(1)-InSet(3), 1-InSet(2)-InSet(4)]);
        hold on;
            
    end
   
    %add object
    ospsurf = f.ospray.addObject(F,V);
    
    if ospsurf.ObjectID == 0
        f.ospray.setCamera(CameraPosition, (CameraTarget-CameraPosition)./norm(CameraTarget-CameraPosition), CameraUpVector, 0.1, CameraFOV)
        f.SizeChangedFcn = @sizeChangedFcn;
        f.Color = [1,1,1];
    end
    
    if numel(f.ospray.Objects) == 1
        drawnowOspray(f);
    end
    
    if holdon
        hold on;
    end

    function sizeChangedFcn(hObject, eventdata, handles)
        f.ospray.dirty = true;
        f.ospray.render([f.Position(4) f.Position(3)]);
        
        for ii = 1:numel(f.CurrentAxes.Children)
            if ~isempty(find(contains(class(f.CurrentAxes.Children(ii)), 'Image')))
                f.CurrentAxes.Children(ii).CData = f.ospray.current_frame;
            end
        end
                
        drawnowOspray(f);
    end

       
end