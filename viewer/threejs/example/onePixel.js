import * as THREE from "three";
import { OrbitControls } from "three/addons/controls/OrbitControls.js";
import Stats from "three/addons/libs/stats.module.js";

// 创建场景、渲染器和相机
const scene = new THREE.Scene();
const renderer = new THREE.WebGLRenderer({ antialias: true });
document.body.appendChild(renderer.domElement);

// 设置屏幕尺寸
const screenWidth = window.innerWidth;
const screenHeight = window.innerHeight;
renderer.setSize(screenWidth, screenHeight);
renderer.setPixelRatio(window.devicePixelRatio);

// 创建透视相机（或正交相机）
const fov = 75; // 对于透视相机的视场角
const aspect = screenWidth / screenHeight;
const camera = new THREE.PerspectiveCamera(fov, aspect, 0.1, 1000);
// 如果使用正交相机，可替换以下配置
// const camera = new THREE.OrthographicCamera(-aspect, aspect, 1, -1, 0.1, 1000);
camera.position.z = 10;
scene.add(camera);

let controls, stats;

controls = new OrbitControls(camera, renderer.domElement);
controls.update();

stats = new Stats();
document.body.appendChild(stats.dom);
// 创建 Plane 的几何体
const geometry = new THREE.PlaneGeometry(1, 1);

// 创建自定义材质
const material = new THREE.ShaderMaterial({
  vertexShader: `
    uniform float screenWidth; // 屏幕宽度
    uniform float screenHeight; // 屏幕高度

    void main() {
      vec4 worldPosition = modelMatrix * vec4(position, 1.0);
      // 计算相机到平面的距离
      float distance = length(worldPosition.xyz - cameraPosition);

      // 1 像素对应的世界空间宽度
      float pixelWorldSize = (distance / screenHeight) * 2.0;

      // 动态调整顶点位置，确保平面宽度为 1 像素
      vec3 scaledPosition = position;
      scaledPosition.x *= pixelWorldSize;

      gl_Position = projectionMatrix * modelViewMatrix * vec4(scaledPosition, 1.0);
    }
  `,
  fragmentShader: `
    void main() {
      gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0); // 红色平面
    }
  `,
  uniforms: {
    screenWidth: { value: window.innerWidth }, // 屏幕宽度
    screenHeight: { value: window.innerHeight }, // 屏幕高度
  },
});

// 创建 Mesh
const plane = new THREE.Mesh(geometry, material);
scene.add(plane);

// 动态更新屏幕尺寸（当窗口大小变化时）
window.addEventListener("resize", () => {
  renderer.setSize(window.innerWidth, window.innerHeight);

  material.uniforms.screenWidth.value = window.innerWidth;
  material.uniforms.screenHeight.value = window.innerHeight;
});

// 渲染循环
function animate() {
  requestAnimationFrame(animate);
  renderer.render(scene, camera);
}
animate();


