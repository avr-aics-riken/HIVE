
SphereGenerator = {}
setmetatable(SphereGenerator, {__index = HiveBaseModule})

SphereGenerator.new = function (varname)
    local this = HiveBaseModule.new(varname)
    this.gen = PrimitiveGenerator()
    setmetatable(this, {__index=SphereGenerator})
    return this
end

function SphereGenerator:Do()
    self:UpdateValue()
    return true
end

function SphereGenerator:PointData()
    local v = self.value
    local radius = v.radius
    return self.gen:Sphere(radius);
end
