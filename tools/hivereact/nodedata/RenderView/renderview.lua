
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
        color_file = "",
        depth_file = "",
        ipcpath = ''
    }
    this.network_ipc = nil
    
    setmetatable(this, {__index=RenderView})
    return this
end

function RenderView:Do()
    local property = self.property
    
    local screensize = property.screensize
    local clearcolor = property.clearcolor
    local color_file = property.color_file
    local depth_file = property.depth_file
    local position = property.position
    local target   = property.target
    local up       = property.up
    local fov      = property.fov
    if self.connection.screensize then
        screensize = self.connection.screensize
    end
    if self.connection.clearcolor then
        clearcolor = self.connection.clearcolor
    end
    if self.connection.color_file then
        color_file = self.connection.color_file
    end
    if self.connection.depth_file then
        depth_file = self.connection.depth_file
    end
    if self.connection.position then
        position = self.connection.position
    end
    if self.connection.target then
        target = self.connection.target
    end
    if self.connection.up then
        up = self.connection.up
    end
    if self.connection.fov then
        fov = self.connection.fov
    end

    self.cam:SetScreenSize(screensize[1], screensize[2])
    self.cam:SetFilename(color_file)
    self.cam:SetDepthFilename(depth_file)
    self.cam:ClearColor(clearcolor[1],clearcolor[2],clearcolor[3],clearcolor[4])
    self.cam:LookAt(
        position[1], position[2], position[3],
        target[1],   target[2],   target[3],
        up[1],       up[2],       up[3],
        fov
    )
    
    local temp = {}
    local targetcam
-- For Object
    if self.connection.RenderObject then
        temp = self.connection.RenderObject
    end
        
-- For Camera
    if self.connection.Camera then
        temp[#temp + 1] = self.connection.Camera
        targetcam = self.connection.Camera
    else
        temp[#temp + 1] = self.cam
        targetcam = self.cam
    end

    render(temp)
    
    local mode = 'jpg'
    -- image save
    local imageBuffer
    local imageBufferSize
    if self.property.ipcpath ~= '' then
        mode = 'raw'
        -- image save
        local img = targetcam:GetImageBuffer()	
        imageBuffer = img:GetBuffer()
        imageBufferSize = img:GetSize()
    else    
        -- image save
        imageBuffer = HIVE_ImageSaver:SaveMemory(1, targetcam:GetImageBuffer())
        imageBufferSize = HIVE_ImageSaver:MemorySize()
    end
    
    -- create metabinary
    local w = property.screensize[1]
    local h = property.screensize[2]
    local json = [[{
            "JSONRPC" : "2.0",
            "method" : "renderedImage",            
            "to" : ]] .. targetClientId ..[[,
            "param" : {
                "type" : "]] .. mode .. [[",
                "width" : "]] .. w .. [[",
                "height" : "]] .. h .. [[",
                "canceled": false,
                "varname": "]] .. self.varname .. [["
            },
            "id":0
    }]]
    HIVE_metabin:Create(json, imageBuffer, imageBufferSize)
    --print('JSON=', json, 'size=', imageBufferSize)
    -- send        
    if self.property.ipcpath ~= '' then       
        if self.network_ipc == nil then
            local ipcAddress = 'ipc:///tmp/HIVE_IPC_' .. self.varname -- .. self.property.ipcpath
            print('IPC open=', ipcAddress);
	        self.network_ipc = require("Network").Connection()
	        local ipcr = self.network_ipc:Connect(ipcAddress)
	        print('IPC ret=', ipcr)
        end
        print('IPC->', self.network_ipc)
        self.network_ipc:SendBinary(HIVE_metabin:BinaryBuffer(), HIVE_metabin:BinaryBufferSize())
        print('IPC SEND!!!!!!!!!!!')			   

    else
        network:SendBinary(HIVE_metabin:BinaryBuffer(), HIVE_metabin:BinaryBufferSize())
        print('WEBSOCKET SEND!!!!!!!!!!!')			   

    end
end
