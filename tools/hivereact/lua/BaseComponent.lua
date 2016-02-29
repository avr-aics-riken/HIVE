
local BaseComponent = {}

BaseComponent.new = function (varname)
    local this = {}
    this.varname = varname
    this.property = {}
    this.connection = {}
    setmetatable(this, {__index = BaseComponent})
    return this;
end

function BaseComponent:ClearConnect()
    self.connection = {}
end

function BaseComponent:Connect(propname, value)
    self.connection[propname] = value
end

function BaseComponent:Set(propname, value)
    self.property[propname] = value
end

function BaseComponent:Do()    
    print('[Warning:BaseComponent][LUA] Need to override Do() method.')
end

return BaseComponent