
SphereGenerator = {}
setmetatable(SphereGenerator, {__index = BaseComponent})

SphereGenerator.new = function (varname)
    local this = BaseComponent.new(varname)
    this.gen = PrimitiveGenerator()
    setmetatable(this, {__index=SphereGenerator})
    return this
end

function SphereGenerator:Do()
    self:UpdateValue()
end

function SphereGenerator:MeshData()
    local v = self.value
    local radius = v.radius
    return self.gen:Sphere(radius);
end
