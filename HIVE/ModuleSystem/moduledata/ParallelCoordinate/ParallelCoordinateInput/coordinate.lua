ParallelCoordinateVolumeInput = {}
setmetatable(ParallelCoordinateVolumeInput, {__index = BaseComponent})

ParallelCoordinateVolumeInput.new = function (varname)
    local this = BaseComponent.new(varname)
    setmetatable(this, {__index=ParallelCoordinateVolumeInput})
    return this
end

function ParallelCoordinateVolumeInput:Do()
    self:UpdateValue()
    local v = self.value
    local vol = v.volume
    if (vol) then
        w = vol:Width()
        h = vol:Height()
        d = vol:Depth()
        c = vol:Component()
        if (w == 0 or h == 0 or d == 0) then
            print('Volume size is zero = ', w, h, d)
            return "Volume size is zero: (" .. w .. ',' .. h .. ',' .. d ..')'
        end
    else
        return "Invalid volume data"
    end
    return true
end

function ParallelCoordinateVolumeInput:coordinate()
    local v = self.value
    return {volume=v.volume, min=v.minval, max=v.maxval}
end
