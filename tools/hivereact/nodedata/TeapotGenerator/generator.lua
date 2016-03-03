
TeapotGenerator = {}
setmetatable(TeapotGenerator, {__index = BaseComponent})

TeapotGenerator.new = function ()
    local this = {}
    this.gen = PrimitiveGenerator()    
    this.property = {
        size = 1
    }
    setmetatable(this, {__index=TeapotGenerator})
    return this
end

function TeapotGenerator:Do()
    self:UpdateValue()
end

function TeapotGenerator:MeshData()
    local v = self.value    
    local size = v.size    
    return self.gen:Teapot(size);
end
