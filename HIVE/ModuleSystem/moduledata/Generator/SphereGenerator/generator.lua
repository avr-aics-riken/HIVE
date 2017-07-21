
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
    local v = self.value
    local radius = v.radius
    self.point = self.gen:Sphere(radius);
    return (self.point ~= nil);
end

function SphereGenerator:PointData()
    return self.point;
end
