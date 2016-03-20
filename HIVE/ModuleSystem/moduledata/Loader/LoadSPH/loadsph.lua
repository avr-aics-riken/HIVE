
LoadSPH = {}
setmetatable(LoadSPH, {__index = HiveBaseModule})

LoadSPH.new = function (varname)
    local this = HiveBaseModule.new(varname);
    this.loader = require('SphLoader')()
    setmetatable(this, {__index=LoadSPH})
    return this
end

function LoadSPH:Do()
    self:UpdateValue()
    return self.loader:Load(self.value.filepath)
end

function LoadSPH:VolumeData()
    return self.loader:VolumeData()
end

function LoadSPH:Size()
    local size = {self.loader:Width(), self.loader:Height(), self.loader:Depth()}
    return size
end

function LoadSPH:Component()
    return self.loader:Component()
end
