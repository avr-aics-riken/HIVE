
LoadVOL = {}
setmetatable(LoadVOL, {__index = BaseComponent})

LoadVOL.new = function (varname)
    local this = BaseComponent.new(varname);
    this.loader = require('VolLoader')()
    setmetatable(this, {__index=LoadVOL})
    return this
end

function LoadVOL:Do()
    self:UpdateValue()
    self.loader:Load(self.value.filepath)
end

function LoadVOL:VolumeData()
    return self.loader:VolumeData()
end

function LoadVOL:Size()
    local size = {self.loader:Width(), self.loader:Height(), self.loader:Depth()}
    return size
end

function LoadVOL:Component()
    return self.loader:Component()
end
