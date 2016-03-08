TransferFunction = {}
setmetatable(TransferFunction, {__index = BaseComponent})

TransferFunction.new = function (varname)
    local this = BaseComponent.new(varname)
    this.property = {
        value = {1,1,1,1}
    }
    setmetatable(this, {__index=TransferFunction})
    return this
end

function TransferFunction:Do()
    -- nothing to do
end

function TransferFunction:color()
    self:UpdateValue()
    local v = self.value
    local value = v.value
    return value
end
