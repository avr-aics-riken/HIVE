import React from "react";
import ReactDOM from "react-dom";
import Core from '../../Core';

// required property ==========================================================
//  aldction and store :
//  setting (optional) : umtimeline setting
// ============================================================================

export default class UMTimeline extends React.Component {
    constructor(props) {
        super(props);

        this.action = props.action;
        this.store  = props.store;

        this.canvas       = props.canvas;
        this.setting      = props.setting;
        this.width        = !this.canvas ? 0 : this.canvas.width;
        this.height       = !this.canvas ? 0 : this.canvas.height;
        this.data         = { contents : [] };
        this.keyRects     = [];
        this.currentframe = 0;

        this.componentDidMount = this.componentDidMount.bind(this);
        this.resizeDraw        = this.resizeDraw.bind(this);
		this.onFrameChange     = this.onFrameChange.bind(this);
        this.intersects        = this.intersects.bind(this);
        this.drawLine          = this.drawLine.bind(this);
        this.drawKey           = this.drawKey.bind(this);
        this.drawArrow         = this.drawArrow.bind(this);
        this.strokeRect        = this.strokeRect.bind(this);
        this.fillRect          = this.fillRect.bind(this);
        this.fillText          = this.fillText.bind(this);
        this.drawSplitter      = this.drawSplitter.bind(this);
        this.drawBounds        = this.drawBounds.bind(this);
        this.drawMeasure       = this.drawMeasure.bind(this);
        this.drawBackground    = this.drawBackground.bind(this);
        this.calcPropRange     = this.calcPropRange.bind(this);
        this.drawProp          = this.drawProp.bind(this);
        this.drawContent       = this.drawContent.bind(this);
        this.drawData          = this.drawData.bind(this);
        this.drawSeekLine      = this.drawSeekLine.bind(this);
        this.draw              = this.draw.bind(this);
        this.setData           = this.setData.bind(this);
        this.setCurrentFrame   = this.setCurrentFrame.bind(this);
        this.splitX            = this.splitX.bind(this);
        this.moveSplit         = this.moveSplit.bind(this);
        this.moveTimeline      = this.moveTimeline.bind(this);
        this.setScale          = this.setScale.bind(this);
        this.click             = this.click.bind(this);
        this.initMouse         = this.initMouse.bind(this);
        this.initData          = this.initData.bind(this);
		this.onRedraw = this.onRedraw.bind(this);
		this.onSelectChanged = this.onSelectChanged.bind(this);
    }

    componentDidMount(){
        // temp setting
        if (!this.canvas) {
            this.canvas = ReactDOM.findDOMNode(this.refs.canvas);
            this.canvas.style.display = "block";
        }
        if (!this.setting) {
            let d = this.initData();
            this.setting = d.setting;
            this.setData(d.data);
        }
        this.initMouse(this.canvas, this);
        this.resizeDraw();

        window.addEventListener('resize', this.resizeDraw, false);
		this.props.store.on(Core.Constants.CHANGE_FRAME, this.onFrameChange);
		this.props.store.on(Core.Constants.KEYFRAME_ADDED, this.onRedraw);
		this.props.store.on(Core.Constants.NODE_SELECT_CHANGED, this.onSelectChanged);
    }

	componentWillUnmount() {
	    window.removeEventListener('resize', this.resizeDraw);
		this.props.store.off(Core.Constants.CHANGE_FRAME, this.onFrameChange);
		this.props.store.off(Core.Constants.KEYFRAME_ADDED, this.onRedraw);
		this.props.store.off(Core.Constants.NODE_SELECT_CHANGED, this.onSelectChanged);
	}

	onSelectChanged(err, node) {
		this.setData(this.props.store.getTimelineData());
	}

	onFrameChange(err, frame) {
		this.setCurrentFrame(Number(frame));
	}

	onRedraw(err, data) {
		this.setData(this.props.store.getTimelineData());
		console.log(this.props.store.getTimelineData())
		this.draw();
	}

    resizeDraw(){
        // if (this.width !== this.canvas.clientWidth || this.height !== this.canvas.clientHeight) {
        if (this.width !== window.innerWidth || this.height !== this.canvas.clientHeight) {
            // this.width = this.canvas.clientWidth;
            this.width = window.innerWidth;
            this.height = this.canvas.clientHeight;
            this.canvas.width = this.width;
            this.canvas.height = this.height;
            console.log(this.width, this.height);
            this.draw();
        }
    }

    intersects(rect1, rect2) {
        //console.log(rect1, rect2);
        var x1 = Math.max(rect1.x, rect2.x),
            y1 = Math.max(rect1.y, rect2.y),
            x2 = Math.min(rect1.x + rect1.w, rect2.x + rect2.w),
            y2 = Math.min(rect1.y + rect1.h, rect2.y + rect2.h);
        if (x1 < x2 && y1 < y2) {
            return {
                x: x1,
                y: y1,
                w: x2 - x1,
                h: y2 - y1
            };
        } else {
            return null;
        }
    }

    drawLine(context, x1, y1, x2, y2, rect) {
        var irect,
        srect = {
            x: x1,
            y: y1,
            w: x2 - x1,
            h: y2 - y1
        };
        if (x1 < x2 && y1 < y2) {
            irect = this.intersects(srect, rect);
            if (irect) {
                context.beginPath();
                context.moveTo(irect.x, irect.y);
                context.lineTo(irect.x + irect.w, irect.y + irect.h);
                context.stroke();
            } else {
                return irect;
            }
        } else {
            if (x1 === x2) {
                srect.x = x2 - 1.0;
                srect.w = 1.0;
            }
            if (y1 === y2) {
                srect.y = y2 - 1.0;
                srect.h = 1.0;
            }
            irect = this.intersects(srect, rect);
            if (irect) {
                context.beginPath();
                context.moveTo(x1, y1);
                context.lineTo(x2, y2);
                context.stroke();
                return rect;
            } else {
                return irect;
            }
        }
    }

    drawKey(context, x, y, r, rect) {
        var irect = this.intersects({
            x: x - r,
            y: y - r,
            w: r * 2,
            h: r * 2
        }, rect);
        if (irect) {
            context.save();
            context.beginPath();
            context.translate(x, y);
            context.rotate(Math.PI / 4);
            context.fillRect(-r, - r, r * 2, r * 2);
            context.strokeRect(-r, - r, r * 2, r * 2);
            context.restore();
            return true;
        } else {
            return false;
        }
    }

    drawArrow(context, x, y, r, isClose, rect) {
        var irect = this.intersects({
            x: x - r,
            y: y - r,
            w: r * 2,
            h: r * 2
        }, rect);
        if (irect) {
            if (isClose) {
                context.beginPath();
                context.moveTo(x - r, y - r);
                context.lineTo(x + r, y);
                context.lineTo(x - r, y + r);
                context.lineTo(x - r, y - r);
                context.fill();
            } else {
                irect = this.intersects({
                    x: x - r,
                    y: y - r,
                    w: r * 2,
                    h: r * 2
                }, rect);
                context.save();
                context.beginPath();
                context.translate(x, y);
                context.rotate(Math.PI / 2);
                context.moveTo(-r, - r);
                context.lineTo(r, 0);
                context.lineTo(-r, r);
                context.lineTo(-r, - r);
                context.fill();
                context.restore();
            }
        }
    }

    strokeRect(context, x, y, w, h, rect) {
        var irect = this.intersects({
            x: x,
            y: y,
            w: w,
            h: h
        }, rect);
        if (irect) {
            context.strokeRect(x, y, w, h);
        }
    }

    fillRect(context, x, y, w, h, rect) {
        var irect = this.intersects({
            x: x,
            y: y,
            w: w,
            h: h
        }, rect);
        if (irect) {
            context.fillRect(irect.x, irect.y, irect.w, irect.h);
        }
    }

    fillText(context, text, x, y, textSize, rect) {
        var irect = this.intersects({
            x: x,
            y: y - textSize,
            w: textSize,
            h: textSize
        }, rect);
        if (irect) {
            context.fillText(text, x, y);
        }
    }

    drawSplitter(rect) {
        var context = this.canvas.getContext('2d'),
            splitx = this.splitX(),
            ss = this.setting.spiltterSize,
            ss2 = ss / 2.0,
            x = splitx - ss,
            lw = this.setting.lineWidth,
            lw2 = lw * 2.0;

        context.lineWidth = ss;
        context.strokeStyle = this.setting.spiltterColor;
        this.drawLine(context, x, ss2, x, this.height - ss2, rect);
        context.lineWidth = lw2;
        context.strokeStyle = this.setting.lineColor;
        this.drawLine(context, x - ss2 - lw2, ss2, x - ss2 - lw2, this.height - ss2, rect);
        context.lineWidth = lw;
        context.strokeStyle = this.setting.lineColor;
        this.strokeRect(context, x - ss2, ss2, this.setting.spiltterSize, this.height - ss2, rect);
    };

    drawBounds(rect) {
        var context = this.canvas.getContext('2d'),
            splitx = this.splitX(),
            lw = this.setting.lineWidth,
            lw2 = lw * 2.0;

        context.lineWidth = lw;
        context.strokeStyle = this.setting.lineColor;
        this.strokeRect(context, context, lw, lw, this.width - lw2, this.height - lw2, rect);
        //this.drawLine(context, splitx, 1, splitx, this.height - 2);
    };

    drawMeasure(rect) {
        var context = this.canvas.getContext('2d'),
            splitx = this.splitX(),
            scale = this.setting.scale,
            offsetX = this.setting.offsetX,
            mh = this.setting.measureHeight,
            i,
            x,
            startFrame,
            endFrame,
            step,
            valueRect;

        startFrame = offsetX / scale;
        endFrame = (this.width + offsetX) / scale;

        step = 10;
        if (scale <= 3) {
            step = 50;
        }
        if (scale <= 0.8) {
            step = 100;
        }
        if (scale < 0.5) {
            step = 500;
        }

        valueRect = JSON.parse(JSON.stringify(rect));
        valueRect.x = splitx;

        startFrame = Math.floor(startFrame - startFrame % step);
        endFrame = Math.floor(endFrame - endFrame % step);

        for (i = startFrame; i < endFrame; i = i + step) {
            x = splitx + i * scale - offsetX - mh / 2;
            context.fillStyle = this.setting.propTextColor;
            context.font = "normal " + mh + "px sans-serif";
            this.fillText(context, String(i), x, mh, mh, valueRect);
        }
    };

    drawBackground(rect) {
        var context = this.canvas.getContext('2d');
        context.fillStyle = this.setting.background;
        this.fillRect(context, rect.x, rect.y, rect.w, rect.h, rect);
    };

    calcPropRange(prop) {
        var result = {
            left: 0,
            right: 0
        },
        splitx = this.splitX(),
            scale = this.setting.scale,
            prekey = null;

        for (let i in prop.data) {
            if (prekey) {
                result.right = splitx + i * scale;
            } else {
                result.left = splitx + i * scale;
            }
            prekey = i;
        }
        return result;
    }

	drawPropBackground(rect, ypos) {
        var context = this.canvas.getContext('2d'),
            i,
            height = 0,
            cs = this.setting.contentSize,
            cs2 = cs / 2.0,
            splitx = this.splitX(),
            splitx_inv = this.width - splitx,
            ss = this.setting.spiltterSize,
            lw = this.setting.lineWidth,
            lw2 = lw * 2.0,
            pdx = this.setting.propPaddingX,
            kr = this.setting.keyRadius,
            scale = this.setting.scale,
            offsetX = this.setting.offsetX,
            offsetY = this.setting.offsetX,
            result = {
                x: 0,
                width: 0,
                height: 0
            },
            valueRect;

        // keys
        context.fillStyle = this.setting.propColor;
        this.fillRect(context, lw2, ypos + lw2, splitx - ss * 2 - lw2, cs, rect);
        context.strokeStyle = this.setting.lineColor;
        context.lineWidth = lw / 2.0;
        this.strokeRect(context, lw2, ypos + lw2, splitx - ss * 2 - lw2, cs, rect);
        result.height = cs;

        // value bounds
        context.fillStyle = this.setting.propColor;
        this.fillRect(context, splitx, ypos + lw2, splitx_inv - lw, cs, rect);
        context.strokeStyle = this.setting.lineColor;
        context.lineWidth = lw;
        this.drawLine(context, splitx, ypos + lw2 + cs, this.width - lw, ypos + lw2 + cs, rect);
		return result;
	}

    drawProp(rect, ypos, content, prop, index) {
        var context = this.canvas.getContext('2d'),
            i,
            height = 0,
            cs = this.setting.contentSize,
            cs2 = cs / 2.0,
            splitx = this.splitX(),
            splitx_inv = this.width - splitx,
            ss = this.setting.spiltterSize,
            lw = this.setting.lineWidth,
            lw2 = lw * 2.0,
            pdx = this.setting.propPaddingX,
            kr = this.setting.keyRadius,
            scale = this.setting.scale,
            offsetX = this.setting.offsetX,
            offsetY = this.setting.offsetX,
            result = {
                x: 0,
                width: 0,
                height: 0
            },
            valueRect;

		if (index === 0) {
	        // keys
	        context.fillStyle = this.setting.propTextColor;
	        context.font = "normal 12px sans-serif";
	        context.fillText(content.name, pdx + lw2, ypos + cs - cs / 4);
	        result.height = cs;
		}

        valueRect = JSON.parse(JSON.stringify(rect));
        valueRect.x = splitx;

        // values
        context.lineWidth = lw;
        var prekey = null;
        for (i in prop.data) {
            if (prekey) {
                context.fillStyle = content.propColor;
                this.fillRect(context, splitx + prekey * scale - offsetX, ypos + lw2, (i - prekey) * scale, cs - lw2, valueRect);
                result.right = splitx + i * scale;
            } else {
                result.left = splitx + i * scale;
            }
            prekey = i;
        }
        return result;
    };

	drawContentsBackground(rect, ypos) {
		var context = this.canvas.getContext('2d'),
			i,
			height = 0,
			offsetX = this.setting.offsetX,
			offsetY = this.setting.offsetX,
			scale = this.setting.scale,
			cs = this.setting.contentSize,
			cs2 = cs / 2.0,
			splitx = this.splitX(),
			splitx_inv = this.width - splitx,
			ss = this.setting.spiltterSize,
			lw = this.setting.lineWidth,
			lw2 = lw * 2.0,
			kr = this.setting.keyRadius,
			ar = this.setting.arrowRadius,
			bounds,
			valueRect;

		height = height + cs;

		// value bounds
		context.fillStyle = this.setting.contentColor;
		this.fillRect(context, splitx, ypos + lw2, splitx_inv - lw, cs, rect);
		context.strokeStyle = this.setting.lineColor;
		context.lineWidth = lw;
		this.drawLine(context, splitx, ypos + lw2 + cs, this.width - lw, ypos + lw2 + cs, rect);

		valueRect = JSON.parse(JSON.stringify(rect));
		valueRect.x = splitx;

		this.drawPropBackground(rect, ypos);
		return height;
	}

    drawContent(rect, ypos, content) {
        var context = this.canvas.getContext('2d'),
            i,
            props = content.props,
            prop,
            height = 0,
            offsetX = this.setting.offsetX,
            offsetY = this.setting.offsetX,
            scale = this.setting.scale,
            cs = this.setting.contentSize,
            cs2 = cs / 2.0,
            splitx = this.splitX(),
            splitx_inv = this.width - splitx,
            ss = this.setting.spiltterSize,
            lw = this.setting.lineWidth,
            lw2 = lw * 2.0,
            kr = this.setting.keyRadius,
            ar = this.setting.arrowRadius,
            propRange = {
                min: Number.MAX_VALUE,
                max: Number.MIN_VALUE
            },
            bounds,
            valueRect;

        // keys
		/*
        context.fillStyle = this.setting.contentColor;
        this.fillRect(context, lw2, ypos + lw2, splitx - ss * 2 - lw2, cs, rect);
        context.strokeStyle = this.setting.lineColor;
        context.lineWidth = lw / 2.0;
        this.strokeRect(context, lw2, ypos + lw2, splitx - ss * 2 - lw2, cs, rect);
        context.fillStyle = this.setting.textColor;
        context.font = "normal 12px sans-serif";
        context.fillText(content.name, lw2 + cs, ypos + cs - cs / 4);
		*/
        height = height + cs;
        //this.drawArrow(context, cs2, ypos + cs2, ar, content.closed, rect);

        this.keyRects.push({
            x: lw2,
            y: ypos + lw2,
            w: cs,
            h: cs
        });

        // value bounds
		/*
        context.fillStyle = this.setting.contentColor;
        this.fillRect(context, splitx, ypos + lw2, splitx_inv - lw, cs, rect);
        context.strokeStyle = this.setting.lineColor;
        context.lineWidth = lw;
        this.drawLine(context, splitx, ypos + lw2 + cs, this.width - lw, ypos + lw2 + cs, rect);
		*/
        valueRect = JSON.parse(JSON.stringify(rect));
        valueRect.x = splitx;

        //if (!content.closed) {
            for (i = 0; i < props.length; i = i + 1) {
                prop = props[i];
                bounds = this.drawProp(rect, ypos, content, prop, i);
                //propRange.min = Math.min(propRange.min, bounds.left);
                //propRange.max = Math.max(propRange.max, bounds.right);
                //height = height + bounds.height;
            }
			for (i = 0; i < props.length; i = i + 1) {
                prop = props[i];
		        context.fillStyle = this.setting.keyColor;
		        for (let k in prop.data) {
		            if (prop.data.hasOwnProperty(k)) {
		                this.drawKey(context, splitx + k * scale - offsetX, ypos + cs2, kr, valueRect);
		            }
		        }
			}
		/*
        } else {
            for (i = 0; i < props.length; i = i + 1) {
                prop = props[i];
                bounds = this.calcPropRange(prop);
                propRange.min = Math.min(propRange.min, bounds.left);
                propRange.max = Math.max(propRange.max, bounds.right);
            }
        }
		*/

        // prop range
        //context.fillStyle = content.color;
        //this.fillRect(context, propRange.min - offsetX, ypos + lw2, propRange.max - propRange.min, cs - lw2, valueRect);

        return height;
    };

    drawData(rect) {
		var i,
			k,
			contents = this.data.contents,
			content,
			height;

		height = this.setting.measureHeight;
		for (i = 0; i < 10; i = i + 1) {
			height = height + this.drawContentsBackground(rect, height)
		}
		if (!this.data.hasOwnProperty("contents")) {
			return;
		}

		height = this.setting.measureHeight;
        this.keyRects = [];
        for (i = 0; i < contents.length; i = i + 1) {
            content = contents[i];
            height = height + this.drawContent(rect, height, content);
        }
    };

    drawSeekLine(rect) {
        var context = this.canvas.getContext('2d'),
            splitx = this.splitX(),
            offsetX = this.setting.offsetX,
            scale = this.setting.scale,
            lw = this.setting.lineWidth,
            lw2 = lw * 2.0,
            valueRect = JSON.parse(JSON.stringify(rect));

        valueRect.x = splitx;
        valueRect.w = valueRect.w - splitx;

        context.strokeStyle = this.setting.seekLineColor;
        context.lineWidth = 1.0;
        this.drawLine(context, splitx + this.currentframe * scale - offsetX, lw2, splitx + this.currentframe * scale - offsetX, this.height - lw2, valueRect);
    };

    draw(rect) {
        console.time('timeline draw');
        if (!rect) {
            rect = {
                x: 0,
                y: 0,
                w: this.width,
                h: this.height
            };
        }
        this.drawBackground(rect);
        this.drawBounds(rect);
        this.drawMeasure(rect);
        this.drawData(rect);
        this.drawSplitter(rect);
        this.drawSeekLine(rect);

        console.timeEnd('timeline draw');
    };

    setData(data) {
		if (!data.hasOwnProperty('contents')) { return; }
		let selects = this.props.store.getSelectedNodeList();
		let contents = [];
		for (let i = 0; i < selects.length; i = i + 1) {
			let varname = selects[i].varname;
			for (let k = 0; k < data.contents.length; k = k + 1) {
				if (data.contents[k].nodeVarname === varname) {
					contents.push(data.contents[k]);
				}
			}
		}
		console.log(this.data, contents);
        this.data.contents = contents;
        this.draw({
            x: 0,
            y: 0,
            w: this.width,
            h: this.height
        });
    };

    setCurrentFrame(frame) {
		let f = Math.floor(frame + 0.5);
		if (f === this.currentframe) { return; }
        this.currentframe = f;
        if (frame < 0) {
            this.currentframe = 0;
        }
        console.log("frame:", this.currentframe);
        this.draw();
    };

    splitX() {
        return this.setting.split;
    }

    isOnSplitter(x, y) {
        var splitx = this.splitX();
        return x < splitx && x > (splitx - this.setting.spiltterSize * 2) && y >= 0 && y < this.height;
    };

    moveSplit(x) {
        if (x > this.setting.contentSize) {
            this.setting.split = x;
            this.draw();
        }
    };

    moveTimeline(mx, my) {
        this.setting.offsetX = this.setting.offsetX + mx;
        this.setting.offsetY = this.setting.offsetY + my;
        //console.log(this.setting.offsetX, this.setting.offsetY)
        this.draw();
    };

    setScale(s) {
        console.log(s);
        if (s > 0 && s < 20.0) {
            this.setting.scale = s;
            this.draw();
        }
    };

    click(x, y) {
        var i,
        keyRect,
        isClosed,
        rect = {
            x: x - 1,
            y: y - 1,
            w: 2,
            h: 2
        };

        //console.log(x, y, rect);
        for (i = 0; i < this.keyRects.length; i = i + 1) {
            keyRect = this.keyRects[i];
            if (this.intersects(keyRect, rect)) {
                isClosed = this.data.contents[i].closed;
                this.data.contents[i].closed = !isClosed;
                this.draw();
                break;
            }
        }
    };

    initMouse(canvas, timeline) {
        var is_key_changing = false,
            is_split_moving = false,
            is_middle_down = false,
            mstate = {
                pre_x: 0
            };

        canvas.addEventListener('click', function(ev) {
            var rect = ev.target.getBoundingClientRect();
            var x = ev.clientX - rect.left;
            var y = ev.clientY - rect.top;
            timeline.click(x, y);
        });
        canvas.addEventListener('mousedown', ((mstate) => {
            return (ev) => {
                var rect,
                splitx,
                x, y;
                if (ev.button === 0) {
                    rect = ev.target.getBoundingClientRect();
                    x = ev.clientX - rect.left;
                    y = ev.clientY - rect.top;
                    splitx = timeline.splitX();
                    if (x > splitx) {
                        is_key_changing = true;
						this.props.action.changeFrame((x - splitx + timeline.setting.offsetX) / timeline.setting.scale);
                    } else if (timeline.isOnSplitter(x, y)) {
                        is_split_moving = true;
                        canvas.style.cursor = "e-resize";
                    }
                } else if (ev.button == 1) {
                    mstate.pre_x = x;
                    is_middle_down = true;
                }
            };
        })(mstate));
        canvas.addEventListener('mousemove', ((mstate) => {
            return (ev) => {
                var splitx;
                var rect = ev.target.getBoundingClientRect();
                var x = ev.clientX - rect.left;
                var y = ev.clientY - rect.top,
                mx = 0;
                if (timeline.isOnSplitter(x, y)) {
                    canvas.style.cursor = "e-resize";
                } else {
                    canvas.style.cursor = "default";
                }
                if (is_key_changing) {
                    splitx = timeline.splitX();
					this.props.action.changeFrame((x - splitx + timeline.setting.offsetX) / timeline.setting.scale);
                } else if (is_split_moving) {
                    rect = ev.target.getBoundingClientRect();
                    x = ev.clientX - rect.left;
                    timeline.moveSplit(x);
                } else if (is_middle_down && mstate.pre_x) {
                    mx = -(x - mstate.pre_x);
                    //console.log(mx, x, mstate.pre_x, mstate);
                    timeline.moveTimeline(mx, 0);
                }
                mstate.pre_x = x;
            };
        })(mstate));
        canvas.addEventListener('mouseup', function(ev) {
            //is_left_down = false;
            is_key_changing = false;
            is_split_moving = false;
            is_middle_down = false;
            canvas.style.cursor = "default";
        });
        canvas.addEventListener('mousewheel', function(ev) {
            var data = ev.wheelDelta;
            if (data > 0) {
                if (timeline.setting.scale > 1.0) {
                    timeline.setScale(timeline.setting.scale + 1.0);
                } else {
                    timeline.setScale(timeline.setting.scale * 2.0);
                }
            } else {
                if (timeline.setting.scale > 1.0) {
                    timeline.setScale(timeline.setting.scale - 1.0);
                } else if (timeline.setting.scale > 0.125) {
                    timeline.setScale(timeline.setting.scale * 0.5);
                }
            }
        });
    }

    initData() {
        var setting = {
            background    : "rgb(30  , 30  , 30)"  ,
            lineColor     : "rgb(0   , 0   , 0)"   ,
            spiltterColor : "rgb(60  , 60  , 60)"  ,
            contentColor  : "rgb(60  , 60  , 60)"  ,
            propColor     : "rgb(50  , 50  , 50)"  ,
            keyColor      : "rgb(200 , 200 , 200)" ,
            textColor     : "rgb(255 , 255 , 255)" ,
            propTextColor : "rgb(200 , 200 , 200)" ,
            seekLineColor : "rgb(255 , 255 , 255)" ,
            propPaddingX: 6,
            lineWidth: 0.5,
            split: 200, // px
            headerSize: 50,
            contentSize: 22,
            arrowRadius: 4,
            keyRadius: 3.5,
            spiltterSize: 3.0,
            scale: 10.0,
            offsetX: 0.0,
            offsetY: 0.0,
            measureHeight: 11.0
        };
        return {setting: setting, data: this.props.store.getTimelineData()};
    }

    render() {
        return (
            <canvas style={{display: "none"}} ref="canvas" />
        );
    }
}
