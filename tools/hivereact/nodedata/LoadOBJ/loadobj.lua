
LoadOBJ = {}
setmetatable(LoadOBJ, {__index = BaseComponent})

LoadOBJ.new = function (varname)
    local this = BaseComponent.new(varname);
    this.loader = require('ObjLoader')()
    setmetatable(this, {__index=LoadOBJ})
    return this
end

function LoadOBJ:Do()
    self:UpdateValue()
    self.loader:Load(self.value.filepath)
end

function LoadOBJ:MeshData()
    return self.loader:MeshData()
end
