FilterImage = {}
setmetatable(FilterImage, {__index = HiveBaseModule})

FilterImage.new = function (varname)
    local this = HiveBaseModule.new(varname)
    setmetatable(this, {__index=FilterImage});
    this.filter = require("ImageFilter")();
    this.res = nil;
    return this
end

function FilterImage:Do()
    self:UpdateValue()
    local v = self.value;
    local image0 = v.image0;
    local image1 = v.image1;
    local ope = v.operation;
    local factor = v.factor;
    if image1 ~= nil then
        self.res = image1;
    end
    if image0 ~= nil then
        self.res = image0;
    end
    if image0 ~= nil and image1 ~= nil and tonumber(ope) > 0 and tonumber(ope) <= 4 then
        if image0:GetWidth() == image1:GetWidth() 
            and image0:GetHeight() == image1:GetHeight()
            and image0:GetFormat() == image1:GetFormat() then
            self.res = self.filter:Filter(ope, image0, image1, factor);
        end
    end
    return true
end

function FilterImage:result()
    return self.res;
end
