
RenderView = {}
setmetatable(RenderView, {__index = BaseComponent})

RenderView.new = function (varname)
    local this = BaseComponent.new(varname)
    this.cam = Camera()    
    this.property = {
        screensize = {512, 512},
        position = {0,0,300},
        target = {0,0,0},
        up = {0,1,0},
        fov = 60,
        clearcolor = {0,0,0,1},
        color_file = "output.jpg",
        depth_file = "",
    }
    setmetatable(this, {__index=RenderView})
    return this
end

function RenderView:Do()
    local property = self.property    
    self.cam:SetScreenSize(property.screensize[1], property.screensize[2])
    self.cam:SetFilename(property.color_file)
    if property.depth_file ~= nil then
        self.cam:SetDepthFilename(property.depth_file)
    end
    self.cam:ClearColor(property.clearcolor[1],property.clearcolor[2],property.clearcolor[3],property.clearcolor[4])
    self.cam:LookAt(
        property.position[1], property.position[2], property.position[3],
        property.target[1], property.target[2], property.target[3],
        property.up[1], property.up[2], property.up[3],
        property.fov
    )
    
    local temp = {}
    local targetcam
-- For Object
    if self.connection.RenderObject then
        temp[#temp + 1] = self.connection.RenderObject
    end
        
-- For Camera
    if self.connection.Camera then
        temp[#temp + 1] = self.connection.Camera
        targetcam = self.connection.Camera
        print('!!!connection CAMERA!!!!')
    else
        temp[#temp + 1] = self.cam
        targetcam = self.cam
        print('!!!internal CAMERA!!!!')        
    end

    render(temp)
    
    -- image save
    local imageBuffer = HIVE_ImageSaver:SaveMemory(1, targetcam:GetImageBuffer())
    local imageBufferSize = HIVE_ImageSaver:MemorySize()

    -- create metabinary
    local w = property.screensize[1]
    local h = property.screensize[2]
    local json = [[{
            "JSONRPC" : "2.0",
            "method" : "renderedImage",            
            "to" : ]] .. targetClientId ..[[,
            "param" : {
                "type" : "jpg",
                "width" : "]] .. w .. [[",
                "height" : "]] .. h .. [[",
                "canceled": false,
                "varname": "]] .. self.varname .. [["
            },
            "id":0
    }]]
    HIVE_metabin:Create(json, imageBuffer, imageBufferSize)
    print('JSON=', json, 'size=', imageBufferSize)
    -- send        
    network:SendBinary(HIVE_metabin:BinaryBuffer(), HIVE_metabin:BinaryBufferSize())
			   
end
