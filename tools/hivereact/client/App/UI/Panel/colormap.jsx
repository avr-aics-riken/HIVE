import React from 'react';
import ReactDOM from "react-dom";

export default class ColorMap extends React.Component {
    constructor(props) {
        super(props);

        this.callback = props.callback;

        // member
        this.send_colormap_change_event = this.send_colormap_change_event.bind(this);
        this.draw_color_map = this.draw_color_map.bind(this);
        this.delete_color_map_ste = this.delete_color_map_step.bind(this);
        this.get_colormap_rgba = this.get_colormap_rgba.bind(this);

        this.componentDidMount = this.componentDidMount.bind(this);
    }

    send_colormap_change_event(color_steps) {
        this.callback(this.get_colormap_rgba(color_steps, 256, 1));
    }

    draw_color_map(context, grad_param, color_steps) {
        var grad_width = grad_param.grad_width,
            grad_height = grad_param.grad_height,
            width = grad_param.width,
            height = grad_param.height,
            padding = grad_param.padding,
            grad,
            i,
            step;

        context.clearRect(0, 0, width, height);

        grad  = context.createLinearGradient(0, 0, 0, height);
        for (i = 0; i < color_steps.length; i = i + 1) {
            grad.addColorStop(color_steps[i].step, color_steps[i].color);
        }
        context.fillStyle = grad;
        context.beginPath();
        context.fillRect(0, padding, grad_width, grad_height);
        context.closePath();

        context.lineWidth = 1.0;
        context.strokeStyle = 'gray';
        for (i = 0; i < color_steps.length; i = i + 1) {
            step = color_steps[i].step;
            context.beginPath();
            context.moveTo(grad_width, step * grad_height + padding);
            context.lineTo(grad_width + 5, step * grad_height);
            context.lineTo(width, step * grad_height);
            context.lineTo(width, step * grad_height + padding * 2);
            context.lineTo(grad_width + 5, step * grad_height + padding * 2);
            context.lineTo(grad_width, step * grad_height + padding);
            context.closePath();
            context.fill();
            context.stroke();
        }
    }

    delete_color_map_step(context, grad_param, color_steps, step_index) {
        color_steps.splice(step_index, 1);
        this.draw_color_map(context, grad_param, color_steps);
        this.send_colormap_change_event(color_steps);
    }

    get_colormap_rgba(color_steps, width, height) {
        var i,
            grad,
            dummy_canvas = document.createElement('canvas'),
            context = dummy_canvas.getContext("2d");
        dummy_canvas.width = width;
        dummy_canvas.height = height;
        context.clearRect(0, 0, width, height);
        grad = context.createLinearGradient(0, 0, width, 0);
        for (i = 0; i < color_steps.length; i = i + 1) {
            grad.addColorStop(color_steps[i].step, color_steps[i].color);
        }
        context.fillStyle = grad;
        context.beginPath();
        context.fillRect(0, 0, width, height);
        context.closePath();
        context.fill();
        return dummy_canvas;
        // return context.getImageData(0, 0, width, height).data;
    }

    componentDidMount(){
        var initial_colormap = [
            {step: 0,   color: 'rgb(255, 0, 0)'},
            {step: 0.5, color: 'rgb(0, 255, 0)'},
            {step: 1.0, color: 'rgb(0, 0, 255)'}
        ];
        var color_map = ReactDOM.findDOMNode(this.refs.canvas),
            context = color_map.getContext("2d"),
            width = 30, // カラーマップ全体の幅（矢印含む
            height = 256 + 10, // カラーマップ全体の高さ（矢印含む
            color_steps = initial_colormap,
            grad_param = {
                grad_width : 18,
                grad_height : 256,
                width : 35,
                height : 256 + 10,
                padding : 5
            },
            dragging_step = null,
            i,
            step;

        color_map.width = width;
        color_map.height = height;
        // 初回描画
        this.draw_color_map(context, grad_param, color_steps);

        // マウス位置にあるstepを探してインデックスを返す.
        function pick_step(e) {
            var rect = color_map.getBoundingClientRect(),
                px = e.clientX - rect.left,
                py = e.clientY - rect.top,
                arrow = {},
                setting_step;
            for (i = 0; i < color_steps.length; i = i + 1) {
                step = color_steps[i].step;
                arrow.x = 0;
                arrow.y = step * grad_param.grad_height - (grad_param.padding + 2);
                arrow.w = grad_param.width;
                arrow.h =  (grad_param.padding + 2) * 2;
                if (arrow.x < px && px < (arrow.x + arrow.w) &&
                    arrow.y < py && py < (Math.max(arrow.y, 0) + arrow.h)) {
                        return i;
                    }
            }
            return -1;
        }

        // ダブルクリック(色の設定).
        color_map.addEventListener('dblclick', (function (e) {
            var picker = document.getElementById('colorpicker'),
                step_index = pick_step(e);
            picker.click();
            picker.onchange = (function (evt) {
                color_steps[step_index].color = picker.value;
                this.draw_color_map(context, grad_param, color_steps);
                this.send_colormap_change_event(color_steps);
            }).bind(this);
        }).bind(this));

        color_map.addEventListener('mousedown', (function (e) {
            var rect = color_map.getBoundingClientRect(),
                px = e.clientX - rect.left,
                py = e.clientY - rect.top,
                deleting_step,
                step_index = pick_step(e);

            if (step_index >= 0) {
                if (e.button === 0) {
                    // 左クリック(移動開始)
                    dragging_step = color_steps[i];
                } else {
                    // 右クリックなど(削除)
                    deleting_step = color_steps[i];
                    this.delete_color_map_step(context, grad_param, color_steps, i);
                    return;
                }
            }

            // 新規追加
            if (!dragging_step && !deleting_step) {
                var step = Math.min(Math.max(py / grad_param.grad_height, 0.0), 1.0);
                color_steps.push({ step : step, color : "rgb(255, 255, 255)"});
                this.draw_color_map(context, grad_param, color_steps);
                dragging_step = color_steps[color_steps.length - 1];
                this.send_colormap_change_event(color_steps);
            }
        }).bind(this));
        window.addEventListener('mousemove', (function (e) {
            var rect = color_map.getBoundingClientRect(),
                px = e.clientX - rect.left,
                py = e.clientY - rect.top;
            if (dragging_step) {
                dragging_step.step = Math.min(Math.max(py / grad_param.grad_height, 0.0), 1.0); // clamp
                this.draw_color_map(context, grad_param, color_steps);
            }
        }).bind(this));
        window.addEventListener('mouseup', (function () {
            if (dragging_step) {
                dragging_step = null;
                this.send_colormap_change_event(color_steps);
            }
        }).bind(this));

        // 初回に1回イベントを投げる。
        this.send_colormap_change_event(color_steps);
    }

    componentWillUnmount(){
    }

    styles(){
        return {
            canvas: {
                width: "35px",
                height: "256px",
                position: "absolute",
                top: "60px",
                right : "10px",
            },
            container: {
                overflow: "hidden"
            },
            picker: {
                position: "absolute",
                top: "36px",
                right: "12px",
                opacity: "0.0"
            }
        };
    }

    render(){
        const styles = this.styles();
        return (
            <div style={styles.container}>
                <canvas ref="canvas" style={styles.canvas}></canvas>
                <input type="color" id="colorpicker" style={styles.picker} />
            </div>
        );
    }
}

