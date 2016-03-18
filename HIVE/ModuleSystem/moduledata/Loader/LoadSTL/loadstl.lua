
LoadSTL = {}
setmetatable(LoadSTL, {__index = BaseComponent})

LoadSTL.new = function (varname)
    local this = BaseComponent.new(varname);
    this.loader = require('StlLoader')()
    setmetatable(this, {__index=LoadSTL})
    return this
end

function LoadSTL:Do()
    self:UpdateValue()
    return self.loader:Load(self.value.filepath)
end

function LoadSTL:MeshData()
    return self.loader:MeshData()
end
