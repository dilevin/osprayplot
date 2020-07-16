function drawnowOspray(osprayFig)
    
    osprayFig.ospray.render([osprayFig.Position(4) osprayFig.Position(3)]);
    
    for ii = 1:numel(osprayFig.CurrentAxes.Children)
        if ~isempty(find(contains(class(osprayFig.CurrentAxes.Children(ii)), 'Image')))
            osprayFig.CurrentAxes.Children(ii).CData = osprayFig.ospray.current_frame;
            osprayFig.CurrentAxes.Children(ii).CDataMapping = 'direct';
        end
    end
    
    drawnow
    
end