ParallelCoordinate = {}
setmetatable(ParallelCoordinate, {__index = BaseComponent})

ParallelCoordinate.new = function (varname)
    local this = BaseComponent.new(varname)
    setmetatable(this, {__index=ParallelCoordinate})
    return this
end

function ParallelCoordinate:Do()
    self:UpdateValue()
    -- nothing to do
end

function ParallelCoordinate:color()
    self:UpdateValue()
    local v = self.value
    local value = v.value
    return value
end
