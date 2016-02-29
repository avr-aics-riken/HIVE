
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
    local size = tonumber(self.property.size)
    if size == nil then
       self.property.size = 1     
    end
end

function TeapotGenerator:MeshData()
    local size = self.property.size    
    return self.gen:Teapot(size);
end
