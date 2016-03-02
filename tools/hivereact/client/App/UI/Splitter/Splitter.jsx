import React    from 'react';
import ReactDOM from 'react-dom';

export default class Splitter extends React.Component {
    constructor(props) {
        super(props);
        this.axis        = props.axis || 'horizontal'; // horizontal or vertical
        this.move        = props.move;                 // boolean
        this.offset      = props.offset || 0;          // if horizon then margin-top, else margin-left
        this.style       = props.style;                // custom css
        this.minSize     = props.minSize || 50;        // min size
        this.maxSize     = props.maxSize || 50;        // max size
        this.defaultSize = props.defaultSize || 50;    // default size

        this.state = {
            dragging: false
        };

        this.HANDLE_SIZE = 8;

        this.componentDidMount = this.componentDidMount.bind(this);
        this.componentWillUnmount = this.componentWillUnmount.bind(this);
        this.onMouseDown = this.onMouseDown.bind(this);
        this.onMouseUp = this.onMouseUp.bind(this);
        this.onMouseMove = this.onMouseMove.bind(this);
        this.calcStyleValue = this.calcStyleValue.bind(this);
        this.merge = this.merge.bind(this);
        this.styles = this.styles.bind(this);
    }

    componentDidMount(){
        // event setting
        if(!this.move){return;}
        let d = this.refs.handle;
        if(this.axis === 'horizontal'){
            let h = d.offsetTop + this.HANDLE_SIZE / 2;
            this.onMouseMove({pageX: d.offsetLeft, pageY: h});
        }else{
            let h = d.offsetLeft + this.HANDLE_SIZE / 2;
            this.onMouseMove({pageX: h, pageY: d.offsetTop});
        }
        window.addEventListener('mouseup', this.onMouseUp, false);
        window.addEventListener('mousemove', this.onMouseMove, false);
    }

    componentWillUnmount(){
    }

    onMouseDown(){
        this.setState({dragging: true});
    }

    onMouseUp(){
        this.setState({dragging: false});
    }

    onMouseMove(eve){
        var offset, x, y, h, h1, h2;
        if(!this.state.dragging || !this.move){return;}
        let e = this.refs.pane1;
        let f = this.refs.pane2;
        if(this.axis === 'horizontal'){
            offset = e.offsetTop;
            x = eve.pageX;
            y = eve.pageY - offset;
            h = window.innerHeight - offset;
            h2 = h - y  - 1 - this.HANDLE_SIZE / 2;
            h1 = h - h2 - 1 - this.HANDLE_SIZE / 2;
            e.style.flexGrow = Math.max(h1, this.minSize);
            f.style.flexGrow = Math.max(h2, this.maxSize);
        }else{
            offset = e.offsetLeft;
            x = eve.pageX - offset;
            y = eve.pageY;
            h = window.innerWidth - offset;
            h2 = h - y  - this.HANDLE_SIZE / 2;
            h1 = h - h2 - this.HANDLE_SIZE / 2;
            e.style.flexGrow = Math.max(h1, this.minSize);
            f.style.flexGrow = Math.max(h2, this.maxSize);
        }
    }

    // css style parser (beta)
    calcStyleValue(value){
        let i;
        if(value === null || value === undefined){return null;}
        i = value;
        if(value.match){
            if(value.match(/%|px/)){
                i = parseFloat(value.replace(/%|px/, ''));
            }
        }
        if(i === null || i === undefined || isNaN(i)){return null;}
        return i;
    }

    merge(into, obj){
        for(let attr in obj){
            into[attr] = obj[attr];
        }
    }

    styles(){
        return this.axis === 'horizontal' ? {
            splitter: {
                width: '100%',
                height: '100%',
                backgroundColor: 'darkred',
                display: 'flex',
                flex: 1,
                flexDirection: 'column',
                position: 'absolute'
            },
            handle: {
                width: '100%',
                height: this.HANDLE_SIZE + 'px',
                boxShadow: '0px 0px 0px 1px gray inset',
                cursor: 'pointer'
            },
            pane: {
                backgroundColor: 'rgba(0, 255, 0, 0.2)'
            }
        } : {
            splitter: {
                width: '100%',
                height: '100%',
                backgroundColor: 'darkblue',
                display: 'flex',
                flexDirection: 'row'
            },
            handle: {
                width: this.HANDLE_SIZE + 'px',
                height: '100%',
                boxShadow: '0px 0px 0px 1px gray inset',
                cursor: 'pointer'
            },
            pane: {
                backgroundColor: 'rgba(0, 255, 0, 0.2)',
                flexGrow: 1
            }
        };
    }

    render(){
        const styles = this.styles();
        if(!this.style){
            this.style = styles.splitter;
        }else{
            this.merge(this.style, styles.splitter);
        }
        var child = this.props.children;
        return (
            <div style={this.style}>
                <div ref="pane1" style={styles.pane}>{child[0]}</div>
                <div ref="handle"
                     style={styles.handle}
                     onMouseDown={this.onMouseDown}
                     onMouseUp={this.onMouseUp}
                />
                <div ref="pane2" style={styles.pane}>{child[1]}</div>
            </div>
        );
    }
}

