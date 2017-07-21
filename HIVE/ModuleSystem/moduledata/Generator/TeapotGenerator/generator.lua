
TeapotGenerator = {}
setmetatable(TeapotGenerator, {__index = HiveBaseModule})

TeapotGenerator.new = function (varname)
    local this = HiveBaseModule.new(varname)
    this.gen = PrimitiveGenerator()    
    setmetatable(this, {__index=TeapotGenerator})
    return this
end

function TeapotGenerator:Do()
    self:UpdateValue()
    local v = self.value    
    local size = v.size    
    self.mesh = self.gen:Teapot(size);
    return (self.mesh ~= nil)
end

function TeapotGenerator:MeshData()
    return self.mesh;
end
