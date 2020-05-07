function drawnowOspray(osprayFig)

    osprayFig.ospray.render([osprayFig.Position(4) osprayFig.Position(3)]);
    osprayFig.CurrentAxes.Children.CData = osprayFig.ospray.current_frame;
    osprayFig.CurrentAxes.Children.CDataMapping = 'direct';
    drawnow
    
end