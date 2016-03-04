'use strict';

Object.defineProperty(exports, "__esModule", {
    value: true
});

var _react = require('react');

var _react2 = _interopRequireDefault(_react);

var _reactDom = require('react-dom');

var _reactDom2 = _interopRequireDefault(_reactDom);

var _Pane = require('./Pane');

var _Pane2 = _interopRequireDefault(_Pane);

var _Resizer = require('./Resizer');

var _Resizer2 = _interopRequireDefault(_Resizer);

var _reactVendorPrefix = require('react-vendor-prefix');

var _reactVendorPrefix2 = _interopRequireDefault(_reactVendorPrefix);

function _interopRequireDefault(obj) { return obj && obj.__esModule ? obj : { default: obj }; }

exports.default = _react2.default.createClass({
    displayName: 'Splitter',
    getInitialState: function getInitialState() {
        return {
            active: false,
            resized: false
        };
    },
    getDefaultProps: function getDefaultProps() {
        return {
            split: 'vertical',
            minSize: 0
        };
    },
    resizerSize: 4,
    componentDidMount: function componentDidMount() {
        document.addEventListener('mouseup', this.onMouseUp);
        document.addEventListener('mousemove', this.onMouseMove);
        var ref1 = this.refs.pane1;
        var ref2 = this.refs.pane2;
        var node1 = _reactDom2.default.findDOMNode(ref1);
        var node2 = _reactDom2.default.findDOMNode(ref2);
        if (ref1 && this.props.defaultSize !== undefined && !this.state.resized) {
            let parentSize = this.props.split === 'vertical' ? node1.parentNode.offsetWidth : node1.parentNode.offsetHeight;
            ref1.setState({
                size: this.props.defaultSize,
                parentSize: parentSize
            });
            ref2.setState({
                size: parentSize - this.props.defaultSize,
                parentSize: parentSize
            });
        }
        setTimeout(function(){
            if (ref2 && this.props.secondPaneSize !== undefined && !this.state.resized) {
                var windowsize = 0;
                var node = _reactDom2.default.findDOMNode(ref2);
                if(this.props.split === 'vertical'){
                    windowsize = node.getBoundingClientRect().width;
                }else{
                    windowsize = node.getBoundingClientRect().height;
                }
                windowsize *= 2;
                ref1.setState({
                    size: windowsize - this.props.secondPaneSize,
                    parentSize: windowsize
                });
                ref2.setState({
                    size: this.props.secondPaneSize,
                    parentSize: windowsize
                });
            }
        }.bind(this), 100);

        window.addEventListener('resize', this.onResize, false);
    },
    componentWillUnmount: function componentWillUnmount() {
        document.removeEventListener('mouseup', this.onMouseUp);
        document.removeEventListener('mousemove', this.onMouseMove);
    },
    onResize: function(){
        this.unFocus();
        if (this.props.onChange) {
            this.props.onChange(newSize);
        }
        var ref1 = this.refs.pane1;
        var ref2 = this.refs.pane2;
        var node1 = _reactDom2.default.findDOMNode(ref1);
        var node2 = _reactDom2.default.findDOMNode(ref2);
        var parentSize = this.props.split === 'vertical' ? node1.parentNode.offsetWidth : node1.parentNode.offsetHeight;
        var newSize = this.props.split === 'vertical' ? node1.offsetWidth : node1.offsetHeight;
        var newSecond = this.props.split === 'vertical' ? node2.offsetWidth : node2.offsetHeight;
        parentSize -= this.resizerSize;
        if(this.props.lockSecondPane && this.props.secondPaneSize){
            if(this.props.secondPaneSize && this.props.lockSecondPane){
                newSecond = Math.max(newSecond, parseInt(this.props.secondPaneSize, 10));
                newSize = parentSize - newSecond;
            }
        }else{
            if(this.props.dontmove){
                newSize = parseInt(this.props.defaultSize, 10);
                newSecond = parentSize - newSize;
            }else{
                if(this.props.minSize){
                    newSize = Math.max(newSize, parseInt(this.props.minSize, 10));
                    newSecond = parentSize - newSize;
                }
                if(this.props.secondPaneSize){
                    newSecond = Math.max(newSecond, parseInt(this.props.secondPaneSize, 10));
                    newSize = parentSize - newSecond;
                }
            }
        }
        ref1.setState({
            size: newSize,
            parentSize: parentSize
        });
        ref2.setState({
            size: newSecond,
            parentSize: parentSize
        });
    },
    onMouseDown: function onMouseDown(event) {
        this.unFocus();
        var position = this.props.split === 'vertical' ? event.clientX : event.clientY;
        if (this.props.onDragStart) {
            this.props.onDragStart();
        }
        this.setState({
            active: true,
            position: position
        });
    },
    onMouseMove: function onMouseMove(event) {
        if (this.state.active) {
            this.unFocus();
            var ref1 = this.refs.pane1;
            var ref2 = this.refs.pane2;
            if (ref1) {
                var node1 = _reactDom2.default.findDOMNode(ref1);
                if (node1.getBoundingClientRect) {
                    var width = node1.getBoundingClientRect().width;
                    var height = node1.getBoundingClientRect().height;
                    var current = this.props.split === 'vertical' ? event.clientX : event.clientY;
                    var size = this.props.split === 'vertical' ? width : height;
                    var position = this.state.position;

                    var newSize = size - (position - current);
                    var newSecond = 0;
                    this.setState({
                        position: current,
                        resized: true
                    });

                    if (newSize < this.props.minSize) {
                        newSize = this.props.minSize;
                    }

                    if (this.props.onChange) {
                        this.props.onChange(newSize);
                    }

                    var parentSize = this.props.split === 'vertical' ? node1.parentNode.offsetWidth : node1.parentNode.offsetHeight;
                    parentSize -= this.resizerSize;
                    if(this.props.dontmove){
                        return;
                        // newSize = Math.max(newSize, this.props.minSize);
                        // newSecond = parentSize - newSize;
                    }else{
                        newSecond = parentSize - newSize;
                    }
                    ref1.setState({
                        size: newSize,
                        parentSize: parentSize
                    });
                    ref2.setState({
                        size: newSecond,
                        parentSize: parentSize
                    });
                }
            }
        }
    },
    onMouseUp: function onMouseUp() {
        if (this.state.active) {
            if (this.props.onDragFinished) {
                this.props.onDragFinished();
            }
            this.setState({
                active: false
            });
        }
    },
    unFocus: function unFocus() {
        if (document.selection) {
            document.selection.empty();
        } else {
            window.getSelection().removeAllRanges();
        }
    },


    merge: function merge(into, obj) {
        for (var attr in obj) {
            into[attr] = obj[attr];
        }
    },

    render: function render() {

        var split = this.props.split;

        var style = {
            display: 'flex',
            flex: 1,
            position: 'relative',
            outline: 'none',
            overflow: 'hidden',
            MozUserSelect: 'text',
            WebkitUserSelect: 'text',
            msUserSelect: 'text',
            userSelect: 'text'
        };

        if (split === 'vertical') {
            this.merge(style, {
                flexDirection: 'row',
                height: '100%',
                position: 'absolute',
                left: 0,
                right: 0
            });
        } else {
            this.merge(style, {
                flexDirection: 'column',
                height: '100%',
                minHeight: '100%',
                position: 'absolute',
                top: 0,
                bottom: 0,
                width: '100%'
            });
        }

        var children = this.props.children;
        var classes = ['SplitPane', split];
        var prefixed = _reactVendorPrefix2.default.prefix({ styles: style });
        var move = this.props.dontmove;
        var overflow = {pane1: this.props.overflow1, pane2: this.props.overflow2};

        return _react2.default.createElement(
            'div',
            { className: classes.join(' '), style: prefixed.styles, ref: 'splitPane' },
            _react2.default.createElement(
                _Pane2.default,
                { ref: 'pane1', key: 'pane1', target: 'pane1',split: split, overflow: overflow},
                children[0]
            ),
            _react2.default.createElement(
                _Resizer2.default,
                { ref: 'resizer', key: 'resizer', onMouseDown: this.onMouseDown, split: split, dontmove: move }),
            _react2.default.createElement(
                _Pane2.default,
                { ref: 'pane2', key: 'pane2', target: 'pane2', split: split, overflow: overflow },
                children[1]
            )
        );
    }
});
module.exports = exports['default'];
