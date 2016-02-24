import React from "react"
import ReactDOM from "react-dom"
import Core from '../../Core'
import Store from './Store.jsx'

export default class Node extends React.Component {
    constructor(props) {
        super(props);

        this.state = {
            nodes : null,
        };

        // this.nodeChanged = this.nodeChanged.bind(this);
        // this.selectChanged = this.selectChanged.bind(this);
        // this.moveNode = this.moveNode.bind(this);
        //
        // this.componentDidMount = this.componentDidMount.bind(this);
        // this.componentWillUnmount = this.componentWillUnmount.bind(this);
        // this.onMouseMove = this.onMouseMove.bind(this);
        // this.onMouseUp = this.onMouseUp.bind(this);
        // this.onMouseDown = this.onMouseDown.bind(this);
        // this.onKeyDown = this.onKeyDown.bind(this);
        // this.onKeyUp = this.onKeyUp.bind(this);
        this.styles = this.styles.bind(this);
    }

    // componentDidMount() {
    //     window.addEventListener('mousemove', this.onMouseMove);
    //     window.addEventListener('mouseup', this.onMouseUp);
    //     window.addEventListener('keydown', this.onKeyDown);
    //     window.addEventListener('keyup', this.onKeyUp);
    //     this.props.store.on(Core.Store.NODE_CHANGED, this.nodeChanged);
    //     this.props.nodeStore.on(Store.NODE_SELECTE_CHANGED, this.selectChanged);
    //     this.props.nodeStore.on(Store.NODE_MOVED, this.moveNode);
    // }
    //
    // componentWillUnmount() {
    //     window.removeEventListener('mousemove', this.onMouseMove);
    //     window.removeEventListener('mouseup', this.onMouseUp);
    //     window.removeEventListener('keydown', this.onKeyDown);
    //     window.removeEventListener('keyup', this.onKeyUp);
    //     this.props.store.removeListener(Core.Store.NODE_CHANGED, this.nodeChanged);
    //     this.props.nodeStore.removeListener(Store.NODE_SELECTE_CHANGED, this.selectChanged);
    //     this.props.nodeStore.removeListener(Store.NODE_MOVED, this.moveNode);
    // }

    styles() {
        return {
            menuArea: {
                backgroundColor: "#222",
                color: "#eee",
                fontSize: "9pt",
                margin: "0px",
                padding: "0px",
                width: "100%",
                height: "100px",
                position: "absolute",
                bottom: "0px",
                left: "0px"
            },
            block: {
                textAlign: "right",
                margin: "0px",
                padding: "2px 0px",
                width: "100%",
                minHeight: "20px",
                boxShadow: "0px 0px 1px 0px white inset"
            },
            blockTitle: {
                backgroundColor: "rgba(0, 0, 0, 0.5)",
                borderRadius: "0px 0px 15px 0px",
                color: "whitesmoke",
                fontSize: "x-small",
                lineHeight: "15px",
                margin: "0px",
                padding: "0px 12px 0px 5px",
                height: "15px",
                display: "inline-block",
                float: "left",
                boxShadow: "0px -1px 1px 1px #666 inset"
            }
        }
    }

    button(value){
        return (
            <div><input type="button" value={value} /></div>
        );
    }

    filer(name){
        return (
            <div><input type="file" name={name} /></div>
        );
    }

    block(value, key){
        const style = this.styles();
        if(value.title === ''){return;}
        return (
            <div style={style.block} key={key}>
                <span style={style.blockTitle}>{value.title}</span>
                {value.item("test button")}
            </div>
        );
    }

    // bl という配列の中で定義した配列のとおりにコントロールを
    // 格納するブロックが生成される感じ（仮）
    render(){
        const style = this.styles();
        let bl = [
            {
                title: 'block1 title',
                item: this.button.bind(this)
            },
            {
                title: 'block2 title',
                item: this.filer.bind(this)
            }
        ];
        return (
            <div style={style.menuArea}>
                {bl.map(this.block.bind(this))}
            </div>
        );
    }
}
