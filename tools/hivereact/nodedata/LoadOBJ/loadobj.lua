
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
function LoadOBJ:PointData()
    return self.loader:PointData()
end
function LoadOBJ:LineData()
    return self.loader:LineData()
end
function LoadOBJ:TetraData()
    return self.loader:TetraData()
end
function LoadOBJ:NormalData()
    return self.loader:NormalData()
end
